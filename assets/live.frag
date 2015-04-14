#version 130
uniform vec3  iResolution;    // viewport resolution (in pixels)
uniform vec3  iColor;
uniform float iGlobalTime;
uniform float iAlpha;

float PI = 3.14159265359;

float RATIO_SPINS_PEED = 5.2;
float RATIO_SPIN_POWER = .01;
float RATIO_DIVIDE = 20.0;
float getAngle(vec2 uvCenter)
{
	float angle = atan(uvCenter.y,uvCenter.x )/(2.0*PI);
	if(angle <0.0) angle +=1.0;
	return angle;
}
float getDis(vec2 v)
{
	return sqrt(v.x* v.x + v.y*v.y);
}			
void main()
{
  vec2 uv = gl_FragCoord.xy / iResolution.xy;
	vec2 uvCenter = uv - vec2(.5,1.0);
	float angle = getAngle(uvCenter);
	float dis = getDis(uvCenter) ;
	angle = angle+ cos(dis*PI - iGlobalTime*RATIO_SPINS_PEED)*RATIO_SPIN_POWER;
	
	float color = cos(angle*2.0*PI*RATIO_DIVIDE);
	gl_FragColor = vec4(iColor.r*color, iColor.g*color,iColor.b*color,iAlpha);  
}
