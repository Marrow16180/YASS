uniform vec2 u_resolution;
uniform float u_time;

// return random noise in the range [0.0, 1.0] as a function of x
float noise2d( in vec2 v )
{
    float xhash = cos( v.x * 37.0 );
    float yhash = cos( v.y * 57.0 );
    return fract( 415.92653 * ( xhash + yhash ) );
}

// convert noise2d() into a "starfield" by stomping everything below threshold to zero
float noisy_starfield( in vec2 sample_pos, float threshold )
{
    float star_val = noise2d( sample_pos );
    if ( star_val >= threshold )
        star_val = pow( (star_val - threshold) / (1.0 - threshold), 6.0 );
    else
        star_val = 0.0;
    return star_val;
}

// stabilize noisy_starfield() by only sampling at integer values
float stable_starfield( in vec2 sample_pos, float threshold )
{
    float fract_x = fract( sample_pos.x );
    float fract_y = fract( sample_pos.y );
    vec2 floor_sample = floor( sample_pos );    
    float v1 = noisy_starfield( floor_sample, threshold );
    float v2 = noisy_starfield( floor_sample + vec2( 0.0, 1.0 ), threshold );
    float v3 = noisy_starfield( floor_sample + vec2( 1.0, 0.0 ), threshold );
    float v4 = noisy_starfield( floor_sample + vec2( 1.0, 1.0 ), threshold );

    float star_val =   v1 * ( 1.0 - fract_x ) * ( 1.0 - fract_y )
        			+ v2 * ( 1.0 - fract_x ) * fract_y
        			+ v3 * fract_x * ( 1.0 - fract_y )
        			+ v4 * fract_x * fract_y;
	return star_val;
}

void main()
{
	// sky background color
	vec3 v_color = vec3( 0.4, 0.2, 0.1 ) * gl_FragCoord.x / u_resolution.x;

	// note: choose threshold in the range [0.99, 0.9999]
    // higher values yield a sparser starfield
    float starfield_threshold = 0.97;

	// stars with a slow crawl
    float x_rate = 0.0;
    float y_rate = 0.2;
    vec2 sample_pos = gl_FragCoord.xy + vec2( x_rate * u_time * 100.0, y_rate * u_time * 100.0 );
	float star_val = stable_starfield( sample_pos, starfield_threshold );
    v_color += vec3( star_val );
	
	gl_FragColor = vec4(v_color, 1.0);
}