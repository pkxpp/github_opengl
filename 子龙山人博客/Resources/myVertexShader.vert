attribute vec3 a_position;  //注意之前我们使用的是vec2
attribute vec4 a_color;

varying vec4 v_fragmentColor;

void main()
{
gl_Position = CC_MVPMatrix * vec4(a_position.xyz,1);  //这里用swizzle的时候是xyz
v_fragmentColor = a_color;
}