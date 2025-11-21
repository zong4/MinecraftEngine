#version 330 core

#define MEDIUMP_FLT_MAX    65504.0
#define saturateMediump(x) min(x, MEDIUMP_FLT_MAX)

// Output
out vec4 FragColor;

struct Light
{
    int  Type;      // 0: Directional, 1: Point, 2: Spot

    vec3 Position;
    vec3 Direction; // FIX: Spotlight / Directional 方向，需与 CPU 侧结构体一致
    vec3 Color;

    float Constant;
    float Linear;
    float Quadratic;

    float CutOff;
    float OuterCutOff;
};

// Lights
#define MAX_LIGHTS 4
uniform int  u_NumLights;
uniform Light u_Light[MAX_LIGHTS];
uniform mat4  u_LightView[MAX_LIGHTS];
uniform mat4  u_LightProjection[MAX_LIGHTS];
uniform sampler2D u_ShadowMap[MAX_LIGHTS];

// Skybox（此处暂未使用）
uniform samplerCube u_Skybox;

// PBR Material Properties
uniform vec3  u_MaterialAlbedo;
uniform float u_MaterialMetallic;
uniform float u_MaterialRoughness;
uniform float u_MaterialAO;

// PBR Textures (optional)
uniform sampler2D u_MaterialAlbedoMap;
uniform sampler2D u_MaterialMetallicMap;
uniform sampler2D u_MaterialRoughnessMap;
uniform sampler2D u_MaterialNormalMap;
uniform sampler2D u_MaterialAOMap;
uniform bool u_MaterialUseAlbedoMap;
uniform bool u_MaterialUseMetallicMap;
uniform bool u_MaterialUseRoughnessMap;
uniform bool u_MaterialUseNormalMap;
uniform bool u_MaterialUseAOMap;

// IBL (Image Based Lighting)
uniform samplerCube u_IrradianceMap;
uniform samplerCube u_PrefilterMap;
uniform sampler2D  u_BRDFLUT;
uniform bool       u_UseIBL;

// Tone mapping
uniform float u_Exposure;

// Inputs
in VS_OUT
{
    vec3 CameraPosition;

    flat uint EntityID;
    vec3 Position;
    vec3 Normal;
    vec3 TexCoord;   // 与 Common.vs 保持一致，使用 .xy 访问 2D 纹理坐标
    vec4 Color;
    vec4 Material;
} fs_in;

// Constants
const float PI = 3.14159265359;

// =======================
// PBR Functions
// =======================

// Normal Distribution Function (Trowbridge-Reitz GGX)
// D(h) = a² / (π * ((n·h)² * (a² - 1) + 1)²)
float DistributionGGX(float NoH, float roughness)
{
    NoH = clamp(NoH, 0.0, 1.0);
    float a  = roughness * roughness;
    float a2 = a * a;

    float denom = NoH * NoH * (a2 - 1.0) + 1.0;
    float D = a2 / max(PI * denom * denom, 0.0001);

    return saturateMediump(D);
}

// Geometry Function (Schlick-GGX)
// G1(v) = n·v / ((n·v) * (1 - k) + k), k = (r+1)² / 8
float GeometrySchlickGGX(float NdotV, float roughness)
{
    NdotV = clamp(NdotV, 0.0, 1.0);
    float r  = roughness + 1.0;
    float k  = (r * r) / 8.0;
    float g1 = NdotV / (NdotV * (1.0 - k) + k);
    return g1;
}

// Geometry Function (Smith)
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);

    float ggx1 = GeometrySchlickGGX(NdotV, roughness);
    float ggx2 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

// Fresnel-Schlick approximation
vec3 FresnelSchlick(vec3 F0, vec3 H, vec3 V)
{
    float HoV = clamp(dot(H, V), 0.0, 1.0);
    return F0 + (1.0 - F0) * pow(1.0 - HoV, 5.0);
}

// Fresnel-Schlick with roughness (for IBL)
vec3 FresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    cosTheta = clamp(cosTheta, 0.0, 1.0);
    vec3 F90 = max(vec3(1.0 - roughness), F0);
    return F0 + (F90 - F0) * pow(1.0 - cosTheta, 5.0);
}

// Calculate PBR lighting for a single light
vec3 CalcPBRLight(
    vec3 N, vec3 V, vec3 L,
    vec3 lightColor,
    vec3 albedo,
    float metallic,
    float roughness,
    vec3 F0
){
    vec3 H = normalize(V + L);

    float NoH = max(dot(N, H), 0.0);
    float NoV = max(dot(N, V), 0.0);
    float NoL = max(dot(N, L), 0.0);

    float D = DistributionGGX(NoH, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    vec3  F = FresnelSchlick(F0, H, V);

    // diffuse term
    vec3 kS = F;
    vec3 kD = (vec3(1.0) - kS) * (1.0 - metallic);

    // specular term
    vec3 numerator = vec3(D * G) * F;
    float denom = max(4.0 * NoV * NoL, 0.0001);
    vec3 specular = numerator / denom;

    vec3 diffuse = kD * albedo / PI;

    return (diffuse + specular) * lightColor * NoL;
}

// Calculate shadow (simple shadow map)
float CalcShadow(int index, vec4 fragPosLightSpace, vec3 lightDir)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    if (projCoords.z > 1.0 ||
        projCoords.x < 0.0 || projCoords.x > 1.0 ||
        projCoords.y < 0.0 || projCoords.y > 1.0)
    {
        return 0.0;
    }

    float closestDepth = texture(u_ShadowMap[index], projCoords.xy).r;
    float currentDepth = projCoords.z;

    float bias = max(0.05 * (1.0 - dot(normalize(fs_in.Normal), normalize(lightDir))), 0.005);
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    return shadow;
}

// =======================
// Main
// =======================
void main()
{
    // -------- 获取材质属性 --------
    vec3 albedo;
    if (u_MaterialUseAlbedoMap)
    {
        // 贴图一般为 sRGB，这里转为线性空间
        albedo = pow(texture(u_MaterialAlbedoMap, fs_in.TexCoord.xy).rgb, vec3(2.2));
    }
    else
    {
        albedo = u_MaterialAlbedo;
    }

    float metallic  = u_MaterialUseMetallicMap
                      ? texture(u_MaterialMetallicMap, fs_in.TexCoord.xy).r
                      : u_MaterialMetallic;

    float roughness = u_MaterialUseRoughnessMap
                      ? texture(u_MaterialRoughnessMap, fs_in.TexCoord.xy).r
                      : u_MaterialRoughness;

    float ao        = u_MaterialUseAOMap
                      ? texture(u_MaterialAOMap, fs_in.TexCoord.xy).r
                      : u_MaterialAO;

    // 顶点颜色叠乘（如果不想叠乘，可以删除这行）
    albedo *= fs_in.Color.rgb;

    // -------- 法线（简单版本，无 TBN）--------
    vec3 N = normalize(fs_in.Normal);
    if (u_MaterialUseNormalMap)
    {
        vec3 normalMap = texture(u_MaterialNormalMap, fs_in.TexCoord.xy).rgb;
        normalMap = normalMap * 2.0 - 1.0; // [0,1] -> [-1,1]
        // TODO: 需要 TBN 矩阵将切线空间法线转换到世界空间
        // 当前实现假设 normalMap 已经在世界空间（可能不正确）
        N = normalize(normalMap);
    }

    // -------- 视线方向 --------
    vec3 V = normalize(fs_in.CameraPosition - fs_in.Position);

    // -------- 基础反射率 F0 --------
    vec3 F0 = mix(vec3(0.04), albedo, metallic);

    // -------- 直接光照 --------
    vec3 Lo = vec3(0.0);

    for (int i = 0; i < u_NumLights && i < MAX_LIGHTS; ++i)
    {
        int type = u_Light[i].Type;
        if (type < 0)
            break;

        vec3 L = vec3(0.0);
        vec3 lightColor = u_Light[i].Color;
        float attenuation = 1.0;

        if (type == 0)
        {
            // Directional Light
            L = normalize(-u_Light[i].Direction);
        }
        else if (type == 1)
        {
            // Point Light
            vec3 lightVec = u_Light[i].Position - fs_in.Position;
            float distance = length(lightVec);
            L = lightVec / max(distance, 0.0001);

            attenuation = 1.0 /
                (u_Light[i].Constant +
                 u_Light[i].Linear * distance +
                 u_Light[i].Quadratic * distance * distance);

            lightColor *= attenuation;
        }
        else if (type == 2)
        {
            // Spot Light
            vec3 lightVec = u_Light[i].Position - fs_in.Position;
            float distance = length(lightVec);
            L = lightVec / max(distance, 0.0001);

            float theta   = dot(normalize(-u_Light[i].Direction), normalize(L));
            float epsilon = u_Light[i].CutOff - u_Light[i].OuterCutOff;
            float intensity = clamp((theta - u_Light[i].OuterCutOff) / max(epsilon, 0.0001), 0.0, 1.0);

            attenuation = 1.0 /
                (u_Light[i].Constant +
                 u_Light[i].Linear * distance +
                 u_Light[i].Quadratic * distance * distance);

            lightColor *= intensity * attenuation;
        }

        // Shadow（只对定向光示例）
        vec4 fragPosLightSpace = u_LightProjection[i] * u_LightView[i] * vec4(fs_in.Position, 1.0);
        float shadow = (type == 0) ? CalcShadow(i, fragPosLightSpace, L) : 0.0;

        Lo += CalcPBRLight(N, V, L, lightColor, albedo, metallic, roughness, F0) * (1.0 - shadow);
    }

    // -------- 环境光（IBL / 简单环境）--------
    vec3 ambient = vec3(0.0);
    float NoV = max(dot(N, V), 0.0);

    if (u_UseIBL)
    {
        // IBL 漫反射
        vec3 irradiance = texture(u_IrradianceMap, N).rgb;
        vec3 diffuse = irradiance * albedo;

        // IBL 镜面
        vec3 R = reflect(-V, N);
        const float MAX_REFLECTION_LOD = 4.0;
        vec3 prefilteredColor = textureLod(u_PrefilterMap, R, roughness * MAX_REFLECTION_LOD).rgb;
        vec2 brdf = texture(u_BRDFLUT, vec2(NoV, roughness)).rg;

        vec3 F = FresnelSchlickRoughness(NoV, F0, roughness);
        vec3 kS = F;
        vec3 kD = (vec3(1.0) - kS) * (1.0 - metallic);

        vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

        ambient = (kD * diffuse + specular) * ao;
    }
    else
    {
        // 简单常量环境光
        ambient = vec3(0.03) * albedo * ao;
    }

    // -------- 合成最终颜色 --------
    vec3 color = ambient + Lo;

    // HDR 色调映射（Reinhard e^-x）
    color = vec3(1.0) - exp(-color * u_Exposure);

    // Gamma 校正
    color = pow(color, vec3(1.0 / 2.2));

    FragColor = vec4(color, fs_in.Color.a);
}
