#version 450

layout( location = 0 ) in vec3 f_Color;
layout( location = 1 ) in vec3 f_Normal;
layout( location = 2 ) in vec3 f_ViewPos;
layout( location = 3 ) in vec3 f_LightPos;

layout( location = 0 ) out vec4 outColor;

void main()
{          
    vec3 r = reflect( f_LightPos, f_Normal );
    
                             //const
    vec3 ambient = f_Color * 0.2;
    vec3 diffuse = max( 0.0, dot( f_Normal, f_LightPos ) ) * f_Color;
                                                        //const   //LightColor
    vec3 specular = pow( max( 0.0, dot( r, f_ViewPos ) ), 16.0 ) * vec3( 1.35 );

    outColor = vec4( ambient + diffuse + specular, 0.7 );
}