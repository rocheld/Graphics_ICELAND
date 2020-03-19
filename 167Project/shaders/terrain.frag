

#version 330 core
// This is a sample fragment shader.
// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
out vec4 fragColor;
uniform vec3 color;
in vec2 TexCoord2;
uniform sampler2D tex;
uniform int flag;
in vec3 normal;

// You can output many things. The first vec4 type output determines the color of the fragment

void main()
{
    
    // Use the color passed in. An alpha of 1.0f means it is not transparent.
    
    fragColor = vec4(color,1.0f);
    fragColor = texture(tex,TexCoord2);
    if(flag == 1)
        fragColor = vec4(normal,1.0f);
    //fragColor = texture(tex,TexCoord2);
}
