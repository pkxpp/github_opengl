attribute vec2 a_position;  //注意之前我们使用的是vec2
attribute vec4 a_color;
attribute vec2 a_coord;

varying vec4 v_fragmentColor;
varying vec2 v_coord;

void main()
{
gl_Position = CC_MVPMatrix * vec4(a_position.xy, 0,1);  //这里用swizzle的时候是xyz
v_fragmentColor = a_color;
v_coord = a_coord;
}