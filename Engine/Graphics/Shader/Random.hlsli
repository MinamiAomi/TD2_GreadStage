float Random(float seed) {
    return frac(sin(seed) * 43758.5453f);
}


float Random(float2 texcood, float seed) {
    return frac(sin(dot(texcood, float2(12.9898f, 78.233f)) + seed) * 43758.5453f);
}

