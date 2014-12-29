uniform float iGlobalTime;
void main()
{
  gl_FragColor = vec4( cos(iGlobalTime), sin(iGlobalTime), 0.1, 1.0 );
 
  
  
}