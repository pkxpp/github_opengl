attribute vec3 a_position;  //ע��֮ǰ����ʹ�õ���vec2
attribute vec4 a_color;

varying vec4 v_fragmentColor;

void main()
{
gl_Position = CC_MVPMatrix * vec4(a_position.xyz,1);  //������swizzle��ʱ����xyz
v_fragmentColor = a_color;
}