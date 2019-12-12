shader_type canvas_item;

uniform sampler2D color_ramp;

uniform bool blobby = false;

void fragment() {
	
	if(blobby) {
		float threshold = texture(TEXTURE, UV).r*2.0 - 1.0;
		if(threshold >= 0.0)
			COLOR = texture(color_ramp, vec2(threshold, 0));
		else
			COLOR = vec4(0.0);
	} else {
		COLOR = texture(TEXTURE, UV);
		COLOR.rgb = texture(color_ramp, vec2(COLOR.r, 0)).rgb;
		COLOR.a = 1.0 - step(COLOR.a, 0.5);
	}
}