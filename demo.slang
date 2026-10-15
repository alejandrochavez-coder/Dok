RWTexture2D<float4> texFrame;
uniform float iTime;
uniform float2 iResolution;
uniform float4 iMouse;
uniform float3 eye;

static const uint MAX_ITERATIONS = 64;

uint mandelbrot(float2 z) {
	float2 c = z;
	
	for (uint i = 0; i < MAX_ITERATIONS; i++) {
		if (dot(z, z) > 256) {
			return i;
		}
		
		z = float2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y)
			+ c;
	}
	return MAX_ITERATIONS;
}

[shader("compute")]
[numthreads(16, 16, 1)]
void main(uint2 thread: SV_DispatchThreadID) {
	float2 uv = thread / iResolution * (sin(2 * iTime) / 2 + 1);
	uv = 4 * uv - 2;
	uint i = mandelbrot(uv);
	float3 color = float(i) / MAX_ITERATIONS * eye;
	color *= sin(i);
	texFrame[thread] = float4(color, 1.0) * iMouse;
}