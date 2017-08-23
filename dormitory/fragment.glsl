// #version 330 core
// out vec4 FragColor;

// float near = 0.1; 
// float far  = 100.0; 

// float LinearizeDepth(float depth) 
// {
//     float z = depth * 2.0 - 1.0; // back to NDC 
//     return (2.0 * near * far) / (far + near - z * (far - near));    
// }

// void main()
// {             
//     float depth = LinearizeDepth(gl_FragCoord.z) / far; // 为了演示除以 far
//     FragColor = vec4(vec3(depth), 1.0);
// }
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

void main()
{    
    FragColor = texture(texture1, TexCoords);
    
}
// void main()
// {
//     FragColor = vec4(0.04, 0.28, 0.26, 1.0);
// }