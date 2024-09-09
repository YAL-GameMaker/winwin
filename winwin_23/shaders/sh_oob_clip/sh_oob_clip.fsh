//
varying vec2 v_vTexcoord;
varying vec4 v_vColour;
varying vec3 v_vPosition;
//
uniform vec3 u_circle;
uniform sampler2D u_texture;
//
void main() {
	vec4 col = v_vColour * texture2D(gm_BaseTexture, v_vTexcoord);
	float f = length(v_vPosition.xy - u_circle.xy) / u_circle.z;
	float a = clamp(1.0 - ((f - 0.9) / 0.1), 0.0, 1.0);
	col.rgba *= a;
	gl_FragColor = col;
}

