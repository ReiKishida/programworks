xof 0303txt 0032

# X File Templates declaration.

template AnimTicksPerSecond
{
 <9e415a43-7ba6-4a73-8743-b73d47e88476>
 DWORD AnimTicksPerSecond;
}
template Frame
{
 <3d82ab46-62da-11cf-ab39-0020af71e433>
 [...]
}
template Matrix4x4
{
 <f6f23f45-7686-11cf-8f52-0040333594a3>
 array FLOAT matrix[16];
}
template FrameTransformMatrix
{
 <f6f23f41-7686-11cf-8f52-0040333594a3>
 Matrix4x4 frameMatrix;
}
template Vector
{
 <3d82ab5e-62da-11cf-ab39-0020af71e433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}
template Coords2d
{
 <f6f23f44-7686-11cf-8f52-0040333594a3>
 FLOAT u;
 FLOAT v;
}
template ColorRGB
{
 <d3e16e81-7835-11cf-8f52-0040333594a3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}
template ColorRGBA
{
 <35ff44e0-6c7c-11cf-8f52-0040333594a3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}
template IndexedColor
{
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}
template MeshFace
{
 <3d82ab5f-62da-11cf-ab39-0020af71e433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}
template MeshNormals
{
 <f6f23f43-7686-11cf-8f52-0040333594a3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}
template MeshVertexColors
{
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}
template MeshTextureCoords
{
 <f6f23f40-7686-11cf-8f52-0040333594a3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}
template Mesh
{
 <3d82ab44-62da-11cf-ab39-0020af71e433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}
template TextureFilename
{
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}
template Material
{
 <3d82ab4d-62da-11cf-ab39-0020af71e433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}
template MeshMaterialList
{
 <f6f23f42-7686-11cf-8f52-0040333594a3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material <3d82ab4d-62da-11cf-ab39-0020af71e433>]
}
template VertexDuplicationIndices
{
 <b8d65549-d7c9-4995-89cf-53a9a8b031e3>
 DWORD nIndices;
 DWORD nOriginalVertices;
 array DWORD indices[nIndices];
}
template XSkinMeshHeader
{
 <3cf169ce-ff7c-44ab-93c0-f78f62d172e2>
 WORD nMaxSkinWeightsPerVertex;
 WORD nMaxSkinWeightsPerFace;
 WORD nBones;
}
template SkinWeights
{
 <6f0d123b-bad2-4167-a0d0-80224f25fabb>
 STRING transformNodeName;
 DWORD nWeights;
 array DWORD vertexIndices[nWeights];
 array FLOAT weights[nWeights];
 Matrix4x4 matrixOffset;
}
template Animation
{
 <3d82ab4f-62da-11cf-ab39-0020af71e433>
 [...]
}
template AnimationSet
{
 <3d82ab50-62da-11cf-ab39-0020af71e433>
 [Animation <3d82ab4f-62da-11cf-ab39-0020af71e433>]
}
template FloatKeys
{
 <10dd46a9-775b-11cf-8f52-0040333594a3>
 DWORD nValues;
 array FLOAT values[nValues];
}
template TimedFloatKeys
{
 <f406b180-7b3b-11cf-8f52-0040333594a3>
 DWORD time;
 FloatKeys tfkeys;
}
template AnimationKey
{
 <10dd46a8-775b-11cf-8f52-0040333594a3>
 DWORD keyType;
 DWORD nKeys;
 array TimedFloatKeys keys[nKeys];
}

# Global definition.


# Materials definition.

Material Material_1
{
 0.800000;0.800000;0.800000;1.000000;;
 5.000000;
 0.300000;0.300000;0.300000;;
 0.000000;0.000000;0.000000;;
 TextureFilename
 {
  "data/TEXTURE/higechan.jpg";
 }
}

# Models definition.

Frame Model
{
 FrameTransformMatrix
 {
  1.000000,0.000000,0.000000,0.000000,
  0.000000,1.000000,0.000000,0.000000,
  0.000000,0.000000,1.000000,0.000000,
  0.000000,0.000000,0.000000,1.000000;;
 }
 Frame Null_1
 {
  FrameTransformMatrix
  {
   0.619019,0.000000,0.000000,0.000000,
   0.000000,0.619019,0.000000,0.000000,
   0.000000,0.000000,0.619019,0.000000,
   0.000000,0.103414,0.019641,1.000000;;
  }
  Frame Null_11
  {
   FrameTransformMatrix
   {
    1.000000,0.000000,0.000000,0.000000,
    0.000000,1.000000,0.000000,0.000000,
    0.000000,0.000000,1.000000,0.000000,
    0.000000,0.000000,0.000000,1.000000;;
   }
  }
 }
 Frame Null_2
 {
  FrameTransformMatrix
  {
   0.707970,0.000000,0.000000,0.000000,
   0.000000,0.100528,0.060872,0.000000,
   0.000000,-0.304147,0.502286,0.000000,
   -0.005238,0.397603,0.248534,1.000000;;
  }
  Frame Null_21
  {
   FrameTransformMatrix
   {
    1.000000,0.000000,0.000000,0.000000,
    0.000000,1.000000,0.000000,0.000000,
    0.000000,0.000000,1.000000,0.000000,
    0.000000,0.000000,0.000000,1.000000;;
   }
  }
 }
 Frame Null_3
 {
  FrameTransformMatrix
  {
   0.128353,-0.029072,0.033437,0.000000,
   0.006687,0.114006,0.073455,0.000000,
   -0.021619,-0.033458,0.053896,0.000000,
   0.042267,0.162249,-0.057912,1.000000;;
  }
  Frame Null_31
  {
   FrameTransformMatrix
   {
    1.000000,0.000000,0.000000,0.000000,
    0.000000,1.000000,0.000000,0.000000,
    0.000000,0.000000,1.000000,0.000000,
    0.000000,0.000000,0.000000,1.000000;;
   }
  }
 }
 Frame Null_4
 {
  FrameTransformMatrix
  {
   0.132863,0.009248,-0.026449,0.000000,
   0.006687,0.114006,0.073455,0.000000,
   0.013429,-0.036117,0.054834,0.000000,
   -0.030791,0.162249,-0.057912,1.000000;;
  }
  Frame Null_41
  {
   FrameTransformMatrix
   {
    1.000000,0.000000,0.000000,0.000000,
    0.000000,1.000000,0.000000,0.000000,
    0.000000,0.000000,1.000000,0.000000,
    0.000000,0.000000,0.000000,1.000000;;
   }
  }
 }
 Frame Null_5
 {
  FrameTransformMatrix
  {
   0.688597,0.000000,0.000000,0.000000,
   0.000000,0.098824,-0.488132,0.000000,
   0.000000,0.159128,0.032216,0.000000,
   0.023063,0.114311,-0.088733,1.000000;;
  }
  Frame Null_51
  {
   FrameTransformMatrix
   {
    1.000000,0.000000,0.000000,0.000000,
    0.000000,1.000000,0.000000,0.000000,
    0.000000,0.000000,1.000000,0.000000,
    0.000000,0.000000,0.000000,1.000000;;
   }
  }
 }
 Frame Null_6
 {
  FrameTransformMatrix
  {
   0.027591,0.000000,0.000000,0.000000,
   0.000000,0.626098,-0.001358,0.000000,
   0.000000,0.000060,0.027590,0.000000,
   -0.231141,0.163608,0.007966,1.000000;;
  }
  Frame Null_61
  {
   FrameTransformMatrix
   {
    1.000000,0.000000,0.000000,0.000000,
    0.000000,1.000000,0.000000,0.000000,
    0.000000,0.000000,1.000000,0.000000,
    0.000000,0.000000,0.000000,1.000000;;
   }
  }
 }
 Frame Null_7
 {
  FrameTransformMatrix
  {
   116.621368,0.000000,0.000000,0.000000,
   0.000000,116.621368,0.000000,0.000000,
   0.000000,0.000000,116.621368,0.000000,
   0.000000,0.000000,0.000000,1.000000;;
  }
  Frame Mesh_71
  {
   FrameTransformMatrix
   {
    1.000000,0.000000,0.000000,0.000000,
    0.000000,1.000000,0.000000,0.000000,
    0.000000,0.000000,1.000000,0.000000,
    0.000000,-0.382234,-0.234495,1.000000;;
   }
   Mesh
   {
    90;
    -0.056179;0.412346;0.193278;,
    0.045704;0.412346;0.193278;,
    -0.077346;0.438820;0.203575;,
    0.066870;0.438820;0.203575;,
    -0.077346;0.376864;0.305892;,
    0.066870;0.376864;0.305892;,
    -0.077346;0.356386;0.293492;,
    0.066870;0.356386;0.293492;,
    0.102924;0.410895;0.211772;,
    0.102924;0.424019;0.219719;,
    0.034092;0.492460;0.254871;,
    -0.044567;0.492460;0.254871;,
    0.034092;0.458668;0.310677;,
    -0.044567;0.458668;0.310677;,
    -0.113400;0.384311;0.285295;,
    0.102924;0.384311;0.285295;,
    0.040977;0.357698;0.329245;,
    -0.051452;0.357698;0.329245;,
    0.040977;0.344574;0.321298;,
    -0.051452;0.344574;0.321298;,
    -0.005238;0.451040;0.160285;,
    -0.005238;0.593326;0.357485;,
    -0.005238;0.344166;0.336782;,
    0.119149;0.397603;0.248534;,
    -0.129624;0.397603;0.248534;,
    -0.005238;0.440398;0.161945;,
    -0.054894;0.447449;0.166214;,
    0.037536;0.421995;0.181072;,
    0.079571;0.427963;0.184686;,
    0.044419;0.447449;0.166214;,
    -0.005238;0.454500;0.170484;,
    0.079571;0.440111;0.192042;,
    0.037536;0.446079;0.195656;,
    -0.090046;0.440111;0.192042;,
    -0.048011;0.446079;0.195656;,
    -0.090046;0.428021;0.225479;,
    -0.005238;0.498013;0.248220;,
    -0.054894;0.476681;0.283450;,
    0.079571;0.428021;0.225479;,
    0.044419;0.476681;0.283450;,
    -0.005238;0.455348;0.318680;,
    0.079571;0.391270;0.286172;,
    0.037536;0.373211;0.315995;,
    -0.090046;0.391270;0.286172;,
    -0.048011;0.373211;0.315995;,
    -0.090046;0.367242;0.312381;,
    -0.005238;0.354807;0.335122;,
    -0.054894;0.347756;0.330853;,
    0.079571;0.367242;0.312381;,
    0.044419;0.347756;0.330853;,
    -0.005238;0.340706;0.326583;,
    0.079571;0.355095;0.305025;,
    0.037536;0.349126;0.301411;,
    -0.090046;0.355095;0.305025;,
    -0.048011;0.349126;0.301411;,
    -0.090046;0.367185;0.271588;,
    0.079571;0.367185;0.271588;,
    0.074325;0.397655;0.210895;,
    -0.083043;0.392895;0.210895;,
    0.110791;0.390552;0.244264;,
    0.110791;0.418935;0.213304;,
    0.110791;0.376270;0.283763;,
    0.110791;0.404654;0.252803;,
    -0.121266;0.390552;0.244264;,
    -0.121266;0.376270;0.283763;,
    -0.121266;0.418935;0.213304;,
    -0.121266;0.404654;0.252803;,
    -0.051452;0.437507;0.167822;,
    -0.005238;0.423797;0.176086;,
    -0.113400;0.410895;0.211772;,
    -0.096315;0.436730;0.183917;,
    0.085840;0.436730;0.183917;,
    -0.005238;0.449663;0.191747;,
    0.040977;0.437507;0.167822;,
    0.040977;0.450632;0.175769;,
    -0.096315;0.410535;0.256364;,
    0.085840;0.410535;0.256364;,
    -0.051452;0.450632;0.175769;,
    -0.005238;0.371408;0.320981;,
    -0.113400;0.424019;0.219719;,
    -0.113400;0.371186;0.277348;,
    -0.096315;0.358476;0.313150;,
    0.085840;0.358476;0.313150;,
    0.102924;0.371186;0.277348;,
    -0.005238;0.345543;0.305320;,
    0.040977;0.362390;0.272022;,
    -0.085147;0.376123;0.240703;,
    0.085840;0.384670;0.240703;,
    -0.044198;0.416326;0.181072;,
    -0.090046;0.427963;0.184686;;
    176;
    3;67,88,0;,
    3;89,67,0;,
    3;25,68,88;,
    3;67,25,88;,
    3;20,25,67;,
    3;26,20,67;,
    3;26,67,89;,
    3;70,26,89;,
    3;73,27,68;,
    3;25,73,68;,
    3;28,1,27;,
    3;73,28,27;,
    3;71,28,73;,
    3;29,71,73;,
    3;29,73,25;,
    3;20,29,25;,
    3;74,29,20;,
    3;30,74,20;,
    3;31,71,29;,
    3;74,31,29;,
    3;3,31,74;,
    3;32,3,74;,
    3;32,74,30;,
    3;72,32,30;,
    3;77,26,70;,
    3;33,77,70;,
    3;30,20,26;,
    3;77,30,26;,
    3;72,30,77;,
    3;34,72,77;,
    3;34,77,33;,
    3;2,34,33;,
    3;11,34,2;,
    3;35,11,2;,
    3;36,72,34;,
    3;11,36,34;,
    3;21,36,11;,
    3;37,21,11;,
    3;37,11,35;,
    3;75,37,35;,
    3;10,32,72;,
    3;36,10,72;,
    3;38,3,32;,
    3;10,38,32;,
    3;76,38,10;,
    3;39,76,10;,
    3;39,10,36;,
    3;21,39,36;,
    3;12,39,21;,
    3;40,12,21;,
    3;41,76,39;,
    3;12,41,39;,
    3;5,41,12;,
    3;42,5,12;,
    3;42,12,40;,
    3;78,42,40;,
    3;13,37,75;,
    3;43,13,75;,
    3;40,21,37;,
    3;13,40,37;,
    3;78,40,13;,
    3;44,78,13;,
    3;44,13,43;,
    3;4,44,43;,
    3;17,44,4;,
    3;45,17,4;,
    3;46,78,44;,
    3;17,46,44;,
    3;22,46,17;,
    3;47,22,17;,
    3;47,17,45;,
    3;81,47,45;,
    3;16,42,78;,
    3;46,16,78;,
    3;48,5,42;,
    3;16,48,42;,
    3;82,48,16;,
    3;49,82,16;,
    3;49,16,46;,
    3;22,49,46;,
    3;18,49,22;,
    3;50,18,22;,
    3;51,82,49;,
    3;18,51,49;,
    3;7,51,18;,
    3;52,7,18;,
    3;52,18,50;,
    3;84,52,50;,
    3;19,47,81;,
    3;53,19,81;,
    3;50,22,47;,
    3;19,50,47;,
    3;84,50,19;,
    3;54,84,19;,
    3;54,19,53;,
    3;6,54,53;,
    3;56,7,52;,
    3;85,56,52;,
    3;8,57,1;,
    3;28,8,1;,
    3;59,87,57;,
    3;8,59,57;,
    3;23,59,8;,
    3;60,23,8;,
    3;60,8,28;,
    3;71,60,28;,
    3;83,56,87;,
    3;59,83,87;,
    3;51,7,56;,
    3;83,51,56;,
    3;82,51,83;,
    3;61,82,83;,
    3;61,83,59;,
    3;23,61,59;,
    3;15,61,23;,
    3;62,15,23;,
    3;48,82,61;,
    3;15,48,61;,
    3;5,48,15;,
    3;41,5,15;,
    3;41,15,62;,
    3;76,41,62;,
    3;9,60,71;,
    3;31,9,71;,
    3;62,23,60;,
    3;9,62,60;,
    3;76,62,9;,
    3;38,76,9;,
    3;38,9,31;,
    3;3,38,31;,
    3;80,55,6;,
    3;53,80,6;,
    3;63,86,55;,
    3;80,63,55;,
    3;24,63,80;,
    3;64,24,80;,
    3;64,80,53;,
    3;81,64,53;,
    3;69,58,86;,
    3;63,69,86;,
    3;89,0,58;,
    3;69,89,58;,
    3;70,89,69;,
    3;65,70,69;,
    3;65,69,63;,
    3;24,65,63;,
    3;79,65,24;,
    3;66,79,24;,
    3;33,70,65;,
    3;79,33,65;,
    3;2,33,79;,
    3;35,2,79;,
    3;35,79,66;,
    3;75,35,66;,
    3;14,64,81;,
    3;45,14,81;,
    3;66,24,64;,
    3;14,66,64;,
    3;75,66,14;,
    3;43,75,14;,
    3;43,14,45;,
    3;4,43,45;,
    3;58,0,88;,
    3;86,58,88;,
    3;55,86,88;,
    3;6,55,88;,
    3;54,6,88;,
    3;84,54,88;,
    3;52,84,88;,
    3;85,52,88;,
    3;56,85,88;,
    3;87,56,88;,
    3;57,87,88;,
    3;1,57,88;,
    3;27,1,88;,
    3;68,27,88;;
    MeshNormals
    {
     90;
     -0.182462;-0.708081;-0.682150;,
     0.131583;-0.725655;-0.675360;,
     -0.350797;0.933138;0.078714;,
     0.274193;0.944055;0.183245;,
     -0.254897;0.623754;0.738890;,
     0.323540;0.546122;0.772705;,
     -0.127660;-0.912179;-0.389400;,
     0.166323;-0.930280;-0.326980;,
     0.455435;-0.618603;-0.640242;,
     0.440630;0.854959;0.273662;,
     0.502222;0.846961;-0.174442;,
     -0.595417;0.718043;-0.360407;,
     0.569077;0.128167;0.812234;,
     -0.468512;0.329311;0.819787;,
     -0.440630;0.638708;0.630791;,
     0.442434;0.637808;0.630439;,
     0.167023;0.462103;0.870956;,
     -0.167091;0.460037;0.872036;,
     0.167092;-0.985935;-0.003537;,
     -0.167023;-0.985936;-0.005868;,
     -0.000000;0.517966;-0.855401;,
     -0.001299;0.851260;0.524743;,
     -0.000000;-0.517966;0.855401;,
     1.000000;-0.000000;-0.000000;,
     -1.000000;-0.000000;-0.000000;,
     0.009215;-0.414169;-0.910153;,
     -0.297627;0.495183;-0.816218;,
     0.075649;-0.730430;-0.678786;,
     0.319985;-0.521515;-0.790968;,
     0.297627;0.493804;-0.817053;,
     -0.000028;0.999287;-0.037751;,
     0.320763;0.939751;0.118237;,
     0.218907;0.974401;-0.051205;,
     -0.322110;0.939117;0.119606;,
     -0.270640;0.952209;-0.141604;,
     -0.530707;0.838145;0.125953;,
     -0.060801;0.877524;-0.475662;,
     -0.783058;0.558912;0.272832;,
     0.467916;0.858938;0.208039;,
     0.785049;0.497575;0.368941;,
     0.058912;0.109925;0.992192;,
     0.495772;0.544665;0.676424;,
     0.242415;0.411527;0.878567;,
     -0.441191;0.610667;0.657598;,
     -0.194928;0.478089;0.856408;,
     -0.320763;0.540274;0.777956;,
     0.000028;0.429638;0.903001;,
     -0.297627;-0.495182;0.816219;,
     0.322110;0.541193;0.776760;,
     0.297627;-0.493804;0.817053;,
     -0.000028;-0.999287;0.037751;,
     0.320764;-0.939750;-0.118236;,
     0.086165;-0.928177;-0.362027;,
     -0.322111;-0.939116;-0.119605;,
     -0.070324;-0.932299;-0.354787;,
     -0.182270;-0.884853;-0.428733;,
     0.153433;-0.888273;-0.432930;,
     0.185533;-0.766430;-0.614949;,
     -0.216293;-0.751485;-0.623288;,
     0.500142;-0.746570;-0.438739;,
     0.913178;0.211769;-0.348224;,
     0.913178;-0.211769;0.348224;,
     0.487877;0.746696;0.452129;,
     -0.535897;-0.732948;-0.419048;,
     -0.913178;-0.210438;0.349031;,
     -0.913178;0.210438;-0.349030;,
     -0.487877;0.746683;0.452151;,
     -0.198581;-0.440003;-0.875764;,
     0.014730;-0.701455;-0.712562;,
     -0.479124;-0.600537;-0.640153;,
     -0.642017;0.397162;-0.655802;,
     0.642017;0.397080;-0.655852;,
     0.000157;0.975116;-0.221696;,
     0.167538;-0.467118;-0.868177;,
     0.167091;0.985935;0.003537;,
     -0.593195;0.689269;0.415966;,
     0.593359;0.689634;0.415126;,
     -0.167023;0.985936;0.005868;,
     -0.000246;0.372108;0.928190;,
     -0.442433;0.854230;0.273028;,
     -0.455403;-0.852525;-0.256533;,
     -0.642017;-0.397161;0.655803;,
     0.642017;-0.397079;0.655852;,
     0.440631;-0.854958;-0.273662;,
     0.002741;-0.939534;-0.342445;,
     0.062704;-0.877118;-0.476164;,
     -0.245199;-0.835484;-0.491776;,
     0.196675;-0.838193;-0.508676;,
     -0.112121;-0.706139;-0.699139;,
     -0.359442;-0.500871;-0.787356;;
     176;
     3;67,88,0;,
     3;89,67,0;,
     3;25,68,88;,
     3;67,25,88;,
     3;20,25,67;,
     3;26,20,67;,
     3;26,67,89;,
     3;70,26,89;,
     3;73,27,68;,
     3;25,73,68;,
     3;28,1,27;,
     3;73,28,27;,
     3;71,28,73;,
     3;29,71,73;,
     3;29,73,25;,
     3;20,29,25;,
     3;74,29,20;,
     3;30,74,20;,
     3;31,71,29;,
     3;74,31,29;,
     3;3,31,74;,
     3;32,3,74;,
     3;32,74,30;,
     3;72,32,30;,
     3;77,26,70;,
     3;33,77,70;,
     3;30,20,26;,
     3;77,30,26;,
     3;72,30,77;,
     3;34,72,77;,
     3;34,77,33;,
     3;2,34,33;,
     3;11,34,2;,
     3;35,11,2;,
     3;36,72,34;,
     3;11,36,34;,
     3;21,36,11;,
     3;37,21,11;,
     3;37,11,35;,
     3;75,37,35;,
     3;10,32,72;,
     3;36,10,72;,
     3;38,3,32;,
     3;10,38,32;,
     3;76,38,10;,
     3;39,76,10;,
     3;39,10,36;,
     3;21,39,36;,
     3;12,39,21;,
     3;40,12,21;,
     3;41,76,39;,
     3;12,41,39;,
     3;5,41,12;,
     3;42,5,12;,
     3;42,12,40;,
     3;78,42,40;,
     3;13,37,75;,
     3;43,13,75;,
     3;40,21,37;,
     3;13,40,37;,
     3;78,40,13;,
     3;44,78,13;,
     3;44,13,43;,
     3;4,44,43;,
     3;17,44,4;,
     3;45,17,4;,
     3;46,78,44;,
     3;17,46,44;,
     3;22,46,17;,
     3;47,22,17;,
     3;47,17,45;,
     3;81,47,45;,
     3;16,42,78;,
     3;46,16,78;,
     3;48,5,42;,
     3;16,48,42;,
     3;82,48,16;,
     3;49,82,16;,
     3;49,16,46;,
     3;22,49,46;,
     3;18,49,22;,
     3;50,18,22;,
     3;51,82,49;,
     3;18,51,49;,
     3;7,51,18;,
     3;52,7,18;,
     3;52,18,50;,
     3;84,52,50;,
     3;19,47,81;,
     3;53,19,81;,
     3;50,22,47;,
     3;19,50,47;,
     3;84,50,19;,
     3;54,84,19;,
     3;54,19,53;,
     3;6,54,53;,
     3;56,7,52;,
     3;85,56,52;,
     3;8,57,1;,
     3;28,8,1;,
     3;59,87,57;,
     3;8,59,57;,
     3;23,59,8;,
     3;60,23,8;,
     3;60,8,28;,
     3;71,60,28;,
     3;83,56,87;,
     3;59,83,87;,
     3;51,7,56;,
     3;83,51,56;,
     3;82,51,83;,
     3;61,82,83;,
     3;61,83,59;,
     3;23,61,59;,
     3;15,61,23;,
     3;62,15,23;,
     3;48,82,61;,
     3;15,48,61;,
     3;5,48,15;,
     3;41,5,15;,
     3;41,15,62;,
     3;76,41,62;,
     3;9,60,71;,
     3;31,9,71;,
     3;62,23,60;,
     3;9,62,60;,
     3;76,62,9;,
     3;38,76,9;,
     3;38,9,31;,
     3;3,38,31;,
     3;80,55,6;,
     3;53,80,6;,
     3;63,86,55;,
     3;80,63,55;,
     3;24,63,80;,
     3;64,24,80;,
     3;64,80,53;,
     3;81,64,53;,
     3;69,58,86;,
     3;63,69,86;,
     3;89,0,58;,
     3;69,89,58;,
     3;70,89,69;,
     3;65,70,69;,
     3;65,69,63;,
     3;24,65,63;,
     3;79,65,24;,
     3;66,79,24;,
     3;33,70,65;,
     3;79,33,65;,
     3;2,33,79;,
     3;35,2,79;,
     3;35,79,66;,
     3;75,35,66;,
     3;14,64,81;,
     3;45,14,81;,
     3;66,24,64;,
     3;14,66,64;,
     3;75,66,14;,
     3;43,75,14;,
     3;43,14,45;,
     3;4,43,45;,
     3;58,0,88;,
     3;86,58,88;,
     3;55,86,88;,
     3;6,55,88;,
     3;54,6,88;,
     3;84,54,88;,
     3;52,84,88;,
     3;85,52,88;,
     3;56,85,88;,
     3;87,56,88;,
     3;57,87,88;,
     3;1,57,88;,
     3;27,1,88;,
     3;68,27,88;;
    }
    MeshTextureCoords
    {
     90;
     0.683587;0.723646;,
     0.842414;0.723646;,
     0.650591;0.682377;,
     0.875411;0.682377;,
     0.650591;0.778961;,
     0.875411;0.778961;,
     0.650591;0.810884;,
     0.875411;0.810884;,
     0.931616;0.725910;,
     0.931616;0.705450;,
     0.824312;0.598757;,
     0.701690;0.598757;,
     0.824312;0.651436;,
     0.701690;0.651436;,
     0.594386;0.767351;,
     0.931616;0.767351;,
     0.835045;0.808838;,
     0.690956;0.808838;,
     0.835045;0.829298;,
     0.690956;0.829298;,
     0.763001;0.663327;,
     0.763001;0.441515;,
     0.763001;0.829934;,
     0.956908;0.746630;,
     0.569094;0.746630;,
     0.763001;0.679916;,
     0.685591;0.668925;,
     0.829680;0.708606;,
     0.895210;0.699301;,
     0.840410;0.668925;,
     0.763001;0.657933;,
     0.895210;0.680365;,
     0.829680;0.671060;,
     0.630792;0.680365;,
     0.696321;0.671060;,
     0.630792;0.699212;,
     0.763001;0.590099;,
     0.685591;0.623355;,
     0.895210;0.699212;,
     0.840410;0.623355;,
     0.763001;0.656610;,
     0.895210;0.756503;,
     0.829680;0.784655;,
     0.630792;0.756503;,
     0.696321;0.784655;,
     0.630792;0.793960;,
     0.763001;0.813345;,
     0.685591;0.824336;,
     0.895210;0.793960;,
     0.840410;0.824336;,
     0.763001;0.835328;,
     0.895210;0.812896;,
     0.829680;0.822201;,
     0.630792;0.812896;,
     0.696321;0.822201;,
     0.630792;0.794049;,
     0.895210;0.794049;,
     0.887032;0.746549;,
     0.641709;0.753970;,
     0.943878;0.757622;,
     0.943878;0.713375;,
     0.943878;0.779886;,
     0.943878;0.735639;,
     0.582123;0.757622;,
     0.582123;0.779886;,
     0.582123;0.713375;,
     0.582123;0.735639;,
     0.690956;0.684423;,
     0.763001;0.705795;,
     0.594386;0.725910;,
     0.621019;0.685635;,
     0.904982;0.685635;,
     0.763001;0.665474;,
     0.835045;0.684423;,
     0.835045;0.663963;,
     0.621019;0.726470;,
     0.904982;0.726470;,
     0.690956;0.663963;,
     0.763001;0.787466;,
     0.594386;0.705450;,
     0.594386;0.787811;,
     0.621019;0.807626;,
     0.904982;0.807626;,
     0.931616;0.787811;,
     0.763001;0.827787;,
     0.835045;0.801523;,
     0.638430;0.780116;,
     0.904982;0.766791;,
     0.702265;0.717442;,
     0.630792;0.699301;;
    }
    MeshMaterialList
    {
     1;
     176;
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0;
     {Material_1}
    }
    VertexDuplicationIndices
    {
     90;
     90;
     0,
     1,
     2,
     3,
     4,
     5,
     6,
     7,
     8,
     9,
     10,
     11,
     12,
     13,
     14,
     15,
     16,
     17,
     18,
     19,
     20,
     21,
     22,
     23,
     24,
     25,
     26,
     27,
     28,
     29,
     30,
     31,
     32,
     33,
     34,
     35,
     36,
     37,
     38,
     39,
     40,
     41,
     42,
     43,
     44,
     45,
     46,
     47,
     48,
     49,
     50,
     51,
     52,
     53,
     54,
     55,
     56,
     57,
     58,
     59,
     60,
     61,
     62,
     63,
     64,
     65,
     66,
     67,
     68,
     69,
     70,
     71,
     72,
     73,
     74,
     75,
     76,
     77,
     78,
     79,
     80,
     81,
     82,
     83,
     84,
     85,
     86,
     87,
     88,
     89;
    }
   }
  }
  Frame Null_72
  {
   FrameTransformMatrix
   {
    1.000000,0.000000,0.000000,0.000000,
    0.000000,1.000000,0.000000,0.000000,
    0.000000,0.000000,1.000000,0.000000,
    0.000000,0.000000,0.000000,1.000000;;
   }
  }
 }
}

# Animations definition.

