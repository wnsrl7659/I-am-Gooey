//
// Simple passthrough vertex shader
//
attribute vec3 in_Position;                  // (x,y,z)
//attribute vec3 in_Normal;                  // (x,y,z)     unused in this shader.
attribute vec4 in_Colour;                    // (r,g,b,a)
attribute vec2 in_TextureCoord;              // (u,v)

varying vec2 v_vTexcoord;
varying vec4 v_vColour;

uniform float UScale;
uniform float VScale;
uniform float TexX;
uniform float TexY;
uniform float TexZ;

uniform float isWall;
varying vec3 v_World;

void main()
{
    vec4 object_space_pos = vec4( in_Position.x, in_Position.y, in_Position.z, 1.0);
    gl_Position = gm_Matrices[MATRIX_WORLD_VIEW_PROJECTION] * object_space_pos;
    
    if(isWall != 0.0)
        v_World = vec3(in_Position.x, TexY + TexZ, TexZ - (in_Position.y - TexY));
    else//Vertex X, Y, and inputted Z for floors. Y GETS AFFECTED BY Z AS WELL
        v_World = vec3(in_Position.x, in_Position.y + TexZ, TexZ);
    
    //calculate distances
    //Loop for each light source
    /*
    for(int i = 0; i < LightCount; i++)
    {
        float xdiff = LightX[i] - PixelX;
        float ydiff = LightY[i] - PixelY;
        float zdiff = LightZ[i] - PixelZ;
        
        ydiff *= 2.0;//Squishes height, makes an elipse
        
        float distSquared = (xdiff * xdiff) + (ydiff * ydiff) + (zdiff * zdiff);
        float radSquared = LightR[i] * LightR[i];
        
        //"Normalizes" the distance
        distSquared /= radSquared;  
        
        if(distSquared <= gloss)
            distSquared = gloss;
        if(distSquared >= gloss)
           distSquared *= (distSquared - (gloss - 1.0)) * 2.0 - 1.0;
    }    */
        
    v_vColour = in_Colour;
    v_vTexcoord = vec2(in_TextureCoord.x * UScale, in_TextureCoord.y * VScale);
}

//######################_==_YOYO_SHADER_MARKER_==_######################@~//
// Simple passthrough fragment shader
//
varying vec2 v_vTexcoord;
varying vec4 v_vColour;

uniform float RealColor[9];  //Colors of the fully lit version

//uniform int LightCount;
const int size = 20;
uniform float LightX[size];
uniform float LightY[size];
uniform float LightZ[size];
uniform float LightR[size];

uniform float LightRed[size];
uniform float LightGreen[size];
uniform float LightBlue[size];

uniform float gloss;

uniform int LightCount;

//uniform float TexX;
//uniform float TexY;
//uniform float TexZ;
uniform float TexSize;
uniform float isWall;

varying vec3 v_World;

void main()
{
    vec4 Original = texture2D( gm_BaseTexture, v_vTexcoord );
    vec4 FinalColor;
    
    //Set everything to black
    FinalColor.r = 0.0;
    FinalColor.g = 0.0;
    FinalColor.b = 0.0;
    FinalColor.a = Original.a;
    
    //Total of all color in original
    float OTotal = Original.r + Original.g + Original.b;
    
    //World Coordinates of the pixel
    float PixelX = v_World.x;
    float PixelY = v_World.y;
    float PixelZ = v_World.z;
    
    float xdiff, ydiff, distSquared, radSquared;
    
    //Loop for each light source
    for(int i = 0; i < LightCount; i++)
    {
        float xdiff = LightX[i] - PixelX;
        float ydiff = LightY[i] - PixelY;
        float zdiff = LightZ[i] - PixelZ;
        
        ydiff *= 2.0;//Squishes height, makes an elipse
        
        float distSquared = (xdiff * xdiff) + (ydiff * ydiff) + (zdiff * zdiff);
        float radSquared = LightR[i] * LightR[i];
        
        //"Normalizes" the distance
        distSquared /= radSquared;  
        
        if(distSquared <= gloss)
            distSquared = gloss;
        if(distSquared >= gloss) //outside main spotlight
            distSquared *= (distSquared - gloss) + 1.0;
        
        //Rough borders
        //distSquared = float(int(distSquared * 5.0))/5.0;       
           
        //Lights colors;
        float red = LightRed[i], green = LightGreen[i] ,blue = LightBlue[i];
        
        if(Original.g > 0.0)//If this pixel has some green value
        {
            //Give the final color the real color of the tile multiplied 
            //by the lights value, the divided by the distance for strength
            //This will make red light only make objects with red value show red etc...
            
            FinalColor.r += RealColor[0] * red / distSquared;  
            FinalColor.g += RealColor[1] * green / distSquared; 
            FinalColor.b += RealColor[2] * blue / distSquared; 
        }
        if(Original.b > 0.0)//If this pixel has some blue value
        {
            FinalColor.r += RealColor[3] * red / distSquared;  
            FinalColor.g += RealColor[4] * green / distSquared; 
            FinalColor.b += RealColor[5] * blue / distSquared; 
        }
        if(Original.r > 0.0)//If this pixel has some red value
        {   
            FinalColor.r += RealColor[6] * red / distSquared;  
            FinalColor.g += RealColor[7] * green / distSquared; 
            FinalColor.b += RealColor[8] * blue / distSquared; 
        }
        /*
        FinalColor.r += red/distSquared;
        FinalColor.g += green/distSquared;
        FinalColor.b += blue/distSquared;
        /**/
    }
    
    //Checkerboard
    /**/
    float VFinal = (FinalColor.r + FinalColor.g + FinalColor.b)/3.0;
    float VReal = 0.0;
    
    if(Original.g > 0.0)//If this pixel has some green value
    {
        for(int i = 0; i < 3; i++)
            VReal += RealColor[i];
        VReal /= 3.0;
    }
    else if(Original.b > 0.0)//If this pixel has some blue value
    {
        for(int i = 0; i < 3; i++)
            VReal += RealColor[i + 3];
        VReal /= 3.0;
    }
    else if(Original.r > 0.0)//If this pixel has some red value
    {   
        for(int i = 0; i < 3; i++)
            VReal += RealColor[i + 6];
        VReal /= 3.0;
    }
    
    float Vdiff = VFinal - VReal;
    //Vdiff:
    //0 -> matches perfectly
    //+ -> final color is brighter than real
    //- -> final color is darker than real
    
    if(true)
    {
        float denominator = 4.0;
        int one = int(v_World.x/denominator);
        if(one < 0) one *= -1;
        
        int two = int(v_World.y/denominator);
        if(two < 0) two *= -1;
        
        int three = int(v_World.z/denominator);
        if(three < 0) three *= -1;
        
        int e = int(mod(float(one + two + three), 2.0));
        //e = float(int(e));
        
        if(mod(float(e), 2.0) != 0.0)
        {
            float temp = FinalColor.a;
            
            if(Vdiff > 0.0)
                Vdiff = 0.0;
            else if(Vdiff < -.5)
                Vdiff = -.5;
            
            FinalColor *= 1.0 + Vdiff;
            FinalColor.a = temp;
            
            //FinalColor = vec4(VReal,VReal,VReal,1.0);
        }
        else
        {
            //FinalColor = vec4(VFinal,VFinal,VFinal,1.0);
        }
    }
    /**/  
    
    /**/
    float Value = (Original.g + Original.b + Original.r)/3.0;
    FinalColor.r *= Value;
    FinalColor.g *= Value;
    FinalColor.b *= Value;
    /**/ 
    
    gl_FragColor = FinalColor;
}    
 
