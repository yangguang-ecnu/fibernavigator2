#include colormap_fs
#include textures_fs
#include uniforms_fs
#include lighting_fs
#include peel_fs

varying float v_value;

void main()
{
    vec3 color = light( u_color ).rgb;
    
    if ( u_colorMode == 0 )
    {
        color = light( u_color ).rgb;
    }
    else if ( u_colorMode == 1 )
    {
        vec4 tcol = texColor( v_texcoord );
    
        if ( !( tcol.r + tcol.g + tcol.b > 0.0 ) ) discard;
        
        color = tcol.rgb;
    }
    else if ( u_colorMode == 3 )
    {
        vec4 mcol = vec4( 0.0, 0.0, 0.0, 1.0 );
        mcol = colormap( v_value, u_colormap, u_lowerThreshold, u_upperThreshold, u_selectedMin, u_selectedMax, 1.0, mcol );
        color = light( mcol ).rgb;
    }
    else
    {
        color = light( gl_Color ).rgb;
    }
    
    writePeel( color );
}
