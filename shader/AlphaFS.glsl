#version 330 core

in vec3 out_FragPos;
in vec3 out_Normal;
in vec2 texcoord;
in float alpha;

out vec4 FragColor;

uniform vec3 LightPos;
uniform vec3 LightColor;

uniform vec3 Ambient;
uniform float Shininess;
uniform vec3 ViewPos;

uniform sampler2D tex;

void main()
{
    vec3 a = Ambient * LightColor;

    vec3 dir = normalize(LightPos - out_FragPos);
    float di = max(dot(out_Normal, dir), 0.0);
    vec3 d = di * LightColor;

    vec3 vd = normalize(ViewPos - out_FragPos);
    vec3 rd = reflect(-dir, out_Normal);
    float si = pow(max(dot(vd, rd), 0.0), Shininess);
    vec3 s = si * LightColor;

    FragColor = vec4(a + d + s, 1.0) * texture(tex, texcoord);
    FragColor.a = alpha;
}