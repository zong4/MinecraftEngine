#version 330 core

// Output
out vec4 FragColor;

struct Light
{
    int Type; // 0: Directional, 1: Point, 2: Spot

    vec3 Position;
    vec3 Color;

    float Constant;
    float Linear;
    float Quadratic;

    float CutOff;
    float OuterCutOff;
};

// Lights
#define MAX_LIGHTS 4
uniform int u_NumLights;
uniform Light u_Light[MAX_LIGHTS];
uniform mat4 u_LightView[MAX_LIGHTS];
uniform mat4 u_LightProjection[MAX_LIGHTS];
uniform sampler2D u_ShadowMap[MAX_LIGHTS];

// Skybox
uniform samplerCube u_Skybox;

// Test
uniform samplerCube u_Texture;

// Inputs
in VS_OUT
{
    vec3 CameraPosition;

    flat uint EntityID;
    vec3 Position;
    vec3 Normal;
    vec3 TexCoord;
    vec4 Color;
    vec4 Material;
}
fs_in;

// Function prototypes
vec3 CalcLight(vec3 lightDir, vec3 viewDir);
float CalcShadow(int index, vec4 fragPosLightSpace, vec3 lightDir);

// Main
void main()
{
    vec3 result = vec3(0.0);

    // Lights
    vec3 viewDir = normalize(fs_in.CameraPosition - fs_in.Position);
    for (int i = 0; i < u_NumLights; ++i)
    {
        int type = u_Light[i].Type;
        if (type == -1)
            break;

        vec3 resultLight = vec3(0.0);
        vec4 lightSpacePosition = u_LightProjection[i] * u_LightView[i] * vec4(fs_in.Position, 1.0);
        if (type == 0)
        {
            vec3 lightDir = normalize(u_Light[i].Position);
            resultLight = CalcLight(lightDir, viewDir) * (1.0 - CalcShadow(i, lightSpacePosition, lightDir));
        }
        else
        {
            vec3 lightDir = normalize(u_Light[i].Position - fs_in.Position);
            resultLight = CalcLight(lightDir, viewDir);

            // Attenuation
            float distance = length(u_Light[i].Position - fs_in.Position);
            float attenuation = 1.0 / (u_Light[i].Constant + u_Light[i].Linear * distance +
                                       u_Light[i].Quadratic * (distance * distance));
            resultLight *= attenuation;

            if (type == 2)
            {
                // Spotlight intensity
                // float theta = dot(lightDir, normalize(-u_Light[i].Direction));
                // float epsilon = u_Light[i].CutOff - u_Light[i].OuterCutOff;
                // float intensity = clamp((theta - u_Light[i].OuterCutOff) / epsilon, 0.0, 1.0);
                // resultLight *= intensity;
            }
        }
        resultLight *= u_Light[i].Color;
        result += resultLight;
    }

    // Skybox
    vec3 resultSkybox = vec3(0.0);
    resultSkybox += texture(u_Skybox, fs_in.Normal).rgb * fs_in.Material[0] * texture(u_Texture, fs_in.TexCoord).rgb *
                    fs_in.Color.rgb;
    // resultSkybox += texture(u_Skybox, reflect(-viewDir, normalize(fs_in.Normal))).rgb *
    // fs_in.Material[2]; resultSkybox += texture(u_Skybox, refract(-viewDir,
    // normalize(fs_in.Normal), 1.00 / 1.52)).rgb * 0.5;
    result += resultSkybox;

    // HDR tonemapping
    float exposure = 1.0;
    result = vec3(1.0) - exp(-result * exposure);

    FragColor = vec4(result, fs_in.Color.a);
}

// No ambient light calculation
vec3 CalcLight(vec3 lightDir, vec3 viewDir)
{
    float diff = max(dot(fs_in.Normal, lightDir), 0.0);

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(fs_in.Normal, halfwayDir), 0.0), fs_in.Material[3]);

    return (fs_in.Material[0] * diff) * texture(u_Texture, fs_in.TexCoord).rgb * fs_in.Color.rgb +
           fs_in.Material[2] * spec;
}

float CalcShadow(int index, vec4 fragPosLightSpace, vec3 lightDir)
{
    // transform to [0,1] range
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    if (projCoords.z > 1.0 || projCoords.x < 0.0 || projCoords.x > 1.0 || projCoords.y < 0.0 || projCoords.y > 1.0)
        return 0.0;

    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(u_ShadowMap[index], projCoords.xy).r;

    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    // check whether current frag pos is in shadow
    float bias = max(0.05 * (1.0 - dot(fs_in.Normal, lightDir)), 0.005);
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    return shadow;
}