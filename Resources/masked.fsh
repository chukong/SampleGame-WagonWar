#ifdef GL_ES
precision lowp float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
    vec4 texColor = texture2D(texture0, v_texCoord);
    vec4 maskColor = texture2D(texture1, v_texCoord);
    vec4 finalColor = vec4(texColor.r, texColor.g, texColor.b, maskColor.r);
    gl_FragColor = finalColor;
}