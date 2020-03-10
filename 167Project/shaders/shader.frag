#version 330 core
// This is a sample fragment shader.
// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in float sampleExtraOutput;
in vec3 normal;
in vec3 FragPos;
vec3 CalcPointLight(vec3 l_pos2,vec3 l_color, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcDirLight(vec3 l_pos,vec3 l_color, vec3 normal, vec3 fragPos, vec3 viewDir);
// Source: https://learnopengl.com/Lighting/Materials

// material
uniform vec3 m_ambient;
uniform vec3 m_diffuse;
uniform vec3 m_specular;
uniform float m_shininess;

// light
uniform vec3 l_pos;
uniform vec3 l_pos2;
uniform vec3 l_color;

uniform vec3 l_ambient = vec3(0.8f,0.0f,0.1f);
uniform vec3 l_diffuse = vec3(0.2f,0.2f,0.2f);
uniform vec3 l_specular = vec3(0.2f,0.2f,0.2f);

uniform vec3 viewDir;
uniform vec3 color;
uniform int flag;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;
void main()
{
    
    // Use the color passed in. An alpha of 1.0f means it is not transparent.
    fragColor = vec4(color, sampleExtraOutput);
    fragColor = vec4(normal,1.0f);
    
    if(flag > 0) {
        vec3 viewPos = vec3(0,0,20);
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 result = CalcDirLight(l_pos,l_color,normal, FragPos,viewDir);
        result += CalcPointLight(l_pos2,l_color,normal, FragPos,viewDir);
        fragColor = vec4(result,1.0);
    }
}

vec3 CalcDirLight(vec3 l_pos,vec3 l_color, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    
    vec3 lightDir = normalize(l_pos - fragPos);
    float diff = max(dot(normal,lightDir), 0.0);
    
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir),0.0), m_shininess);
    
    vec3 ambient = l_ambient * m_ambient;
    vec3 diffuse = l_ambient * diff * m_diffuse;
    vec3 specular = l_ambient * spec * m_specular;
    return (ambient + diffuse + specular);
}
vec3 CalcPointLight(vec3 l_pos2,vec3 l_color, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(l_pos2 - fragPos);
    float diff = max(dot(normal,lightDir), 0.0);
    
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir),0.0), m_shininess);
    
    float distance = length(l_pos2-fragPos);
    float attenuation = 1.0 / (1.0f + 0.09f*distance + 0.032f * (distance*distance));
    
    vec3 ambient = l_ambient;
    vec3 diffuse = l_diffuse;
    vec3 specular = l_specular;
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    return (ambient + diffuse + specular);
}

