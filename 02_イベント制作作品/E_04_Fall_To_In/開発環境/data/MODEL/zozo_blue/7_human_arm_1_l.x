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
 20.000000;
 0.500000;0.500000;0.500000;;
 0.000000;0.000000;0.000000;;
 TextureFilename
 {
  "data\\TEXTURE\\chara_blue.jpg";
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
   1.000000,0.000000,0.000000,0.000000,
   0.000000,1.000000,0.000000,0.000000,
   0.000000,0.000000,1.000000,0.000000,
   0.000000,0.000000,0.000000,1.000000;;
  }
  Frame Mesh_11
  {
   FrameTransformMatrix
   {
    1.000000,0.000000,0.000000,0.000000,
    0.000000,1.000000,0.000000,0.000000,
    0.000000,0.000000,1.000000,0.000000,
    -6.932000,-12.940000,-1.436000,1.000000;;
   }
   Mesh
   {
    158;
    6.552188;14.634960;0.654341;,
    8.019439;11.517944;3.963388;,
    7.826947;12.191541;4.441066;,
    8.027539;11.120801;3.034230;,
    8.484683;14.908997;0.925623;,
    8.601779;14.211645;0.225603;,
    7.094025;13.781006;-0.149566;,
    6.782512;15.256946;1.931559;,
    8.531060;15.282202;1.844114;,
    7.119563;15.095470;3.643250;,
    6.953980;15.337231;2.853018;,
    8.830585;15.067516;3.553932;,
    8.678409;15.322167;2.763961;,
    10.052075;14.897143;2.712208;,
    9.974795;14.884179;1.924208;,
    10.083993;14.655847;3.398550;,
    11.343408;14.260839;3.327451;,
    11.365371;14.423901;2.675734;,
    8.911744;14.564929;4.151432;,
    10.063878;14.207019;3.958591;,
    11.292615;13.918676;3.858905;,
    13.396569;13.725134;3.631069;,
    13.389829;13.989490;3.107748;,
    10.006371;13.590259;4.323819;,
    11.252321;13.436059;4.210455;,
    13.389904;13.294274;3.978553;,
    15.347967;13.121984;3.448083;,
    15.344851;13.459069;3.084317;,
    11.225693;12.837414;4.236578;,
    13.319961;12.719800;3.940970;,
    15.263259;12.627038;3.409872;,
    13.200332;12.151380;3.659779;,
    15.161407;12.151086;3.201897;,
    15.075546;11.708667;2.876451;,
    15.299561;13.645752;2.607995;,
    15.228796;13.723267;2.053504;,
    13.351550;14.070812;2.457879;,
    15.158545;13.717944;1.430671;,
    13.264593;14.030078;1.700438;,
    11.302056;14.391955;1.912748;,
    13.121189;13.803821;0.961850;,
    11.160019;14.078075;1.170586;,
    9.883070;14.532421;1.141506;,
    11.027956;13.611499;0.594527;,
    9.823706;13.954809;0.539077;,
    10.907842;13.046008;0.181489;,
    9.761456;13.285522;0.171508;,
    8.708991;13.435425;-0.119417;,
    9.718234;12.559094;0.091538;,
    8.797334;12.594811;-0.154042;,
    7.635096;12.318640;-0.308141;,
    7.438554;12.993945;-0.390530;,
    7.689657;11.234754;0.929434;,
    7.803779;11.052136;1.928802;,
    7.714962;11.671557;0.147705;,
    8.799657;11.841992;0.237801;,
    8.779617;11.303052;0.934125;,
    9.703026;11.351624;1.020010;,
    9.737149;11.128881;1.849166;,
    8.892789;11.073230;1.893402;,
    9.703761;11.844885;0.385051;,
    10.835983;11.747510;0.387885;,
    10.863447;11.384656;0.999678;,
    10.833980;12.366834;0.084514;,
    12.931394;12.263965;-0.019485;,
    12.947193;11.734887;0.351165;,
    12.942205;12.860718;-0.000755;,
    15.092641;12.768395;0.153890;,
    15.088684;12.292687;0.224307;,
    12.993545;13.400829;0.362501;,
    15.088061;13.210071;0.430098;,
    15.103627;13.552636;0.854564;,
    15.074725;11.883825;0.569185;,
    15.049480;11.616613;1.078151;,
    12.952978;11.396070;0.943366;,
    15.026694;11.419226;1.703058;,
    12.945148;11.178052;1.673329;,
    10.876190;11.195654;1.756011;,
    12.960938;11.192554;2.488191;,
    10.947625;11.230542;2.645668;,
    9.890405;11.169995;2.851571;,
    11.095949;11.654443;3.425412;,
    10.039719;11.587121;3.683068;,
    9.137416;11.534790;3.878620;,
    9.060476;11.141442;2.992043;,
    10.044744;12.179688;4.220929;,
    9.083032;12.173657;4.392206;,
    10.000323;12.872803;4.434225;,
    8.980976;12.993603;4.611293;,
    7.656802;13.055176;4.651121;,
    8.954720;13.844397;4.531570;,
    7.456570;13.919457;4.588035;,
    7.273962;14.599999;4.245030;,
    11.198339;12.232788;3.958221;,
    13.059148;11.589368;3.184145;,
    15.030865;11.414014;2.354276;,
    16.729509;12.386511;1.850606;,
    6.537994;12.854015;2.296710;,
    13.389904;13.294274;3.978553;,
    11.252321;13.436059;4.210455;,
    15.347967;13.121984;3.448083;,
    15.263259;12.627038;3.409872;,
    15.347967;13.121984;3.448083;,
    15.161407;12.151086;3.201897;,
    15.344851;13.459069;3.084317;,
    15.299561;13.645752;2.607995;,
    15.228796;13.723267;2.053504;,
    9.823706;13.954809;0.539077;,
    8.601779;14.211645;0.225603;,
    9.761456;13.285522;0.171508;,
    7.094025;13.781006;-0.149566;,
    8.797334;12.594811;-0.154042;,
    7.635096;12.318640;-0.308141;,
    9.703761;11.844885;0.385051;,
    8.799657;11.841992;0.237801;,
    10.835983;11.747510;0.387885;,
    10.833980;12.366834;0.084514;,
    10.907842;13.046008;0.181489;,
    12.931394;12.263965;-0.019485;,
    12.942205;12.860718;-0.000755;,
    15.092641;12.768395;0.153890;,
    15.088061;13.210071;0.430098;,
    15.092641;12.768395;0.153890;,
    15.103627;13.552636;0.854564;,
    15.158545;13.717944;1.430671;,
    15.088684;12.292687;0.224307;,
    15.074725;11.883825;0.569185;,
    15.049480;11.616613;1.078151;,
    15.088684;12.292687;0.224307;,
    9.083032;12.173657;4.392206;,
    10.044744;12.179688;4.220929;,
    7.826947;12.191541;4.441066;,
    8.911744;14.564929;4.151432;,
    7.273962;14.599999;4.245030;,
    10.063878;14.207019;3.958591;,
    10.006371;13.590259;4.323819;,
    11.198339;12.232788;3.958221;,
    13.200332;12.151380;3.659779;,
    15.161407;12.151086;3.201897;,
    15.075546;11.708667;2.876451;,
    15.030865;11.414014;2.354276;,
    16.729509;12.386511;1.850606;,
    8.019439;11.517944;3.963388;,
    7.826947;12.191541;4.441066;,
    7.119563;15.095470;3.643250;,
    6.953980;15.337231;2.853018;,
    7.438554;12.993945;-0.390530;,
    7.635096;12.318640;-0.308141;,
    7.689657;11.234754;0.929434;,
    7.803779;11.052136;1.928802;,
    7.656802;13.055176;4.651121;,
    7.456570;13.919457;4.588035;,
    8.027539;11.120801;3.034230;,
    6.552188;14.634960;0.654341;,
    7.094025;13.781006;-0.149566;,
    6.782512;15.256946;1.931559;,
    7.714962;11.671557;0.147705;,
    7.273962;14.599999;4.245030;;
    192;
    3;5,6,0;,
    3;4,5,0;,
    3;8,4,0;,
    3;7,8,0;,
    3;11,12,10;,
    3;9,11,10;,
    3;8,7,10;,
    3;12,8,10;,
    3;14,8,12;,
    3;13,14,12;,
    3;15,13,12;,
    3;11,15,12;,
    3;17,13,15;,
    3;16,17,15;,
    3;18,19,15;,
    3;11,18,15;,
    3;20,16,15;,
    3;19,20,15;,
    3;22,16,20;,
    3;21,22,20;,
    3;23,24,20;,
    3;19,23,20;,
    3;25,21,20;,
    3;24,25,20;,
    3;27,21,25;,
    3;26,27,25;,
    3;28,29,98;,
    3;99,28,98;,
    3;30,100,98;,
    3;29,30,98;,
    3;96,102,101;,
    3;31,32,30;,
    3;29,31,30;,
    3;103,96,101;,
    3;103,33,96;,
    3;34,104,96;,
    3;104,102,96;,
    3;96,35,34;,
    3;36,22,105;,
    3;106,36,105;,
    3;21,27,105;,
    3;22,21,105;,
    3;37,38,36;,
    3;106,37,36;,
    3;39,17,36;,
    3;38,39,36;,
    3;16,22,36;,
    3;17,16,36;,
    3;40,41,39;,
    3;38,40,39;,
    3;42,14,39;,
    3;41,42,39;,
    3;13,17,39;,
    3;14,13,39;,
    3;43,44,42;,
    3;41,43,42;,
    3;5,4,42;,
    3;44,5,42;,
    3;8,14,42;,
    3;4,8,42;,
    3;45,46,44;,
    3;43,45,44;,
    3;47,108,107;,
    3;109,47,107;,
    3;48,49,47;,
    3;109,48,47;,
    3;50,51,47;,
    3;49,50,47;,
    3;110,108,47;,
    3;51,110,47;,
    3;56,52,54;,
    3;55,56,54;,
    3;111,55,54;,
    3;112,111,54;,
    3;58,59,56;,
    3;57,58,56;,
    3;53,52,56;,
    3;59,53,56;,
    3;60,57,56;,
    3;55,60,56;,
    3;62,57,60;,
    3;61,62,60;,
    3;49,48,113;,
    3;114,49,113;,
    3;63,115,113;,
    3;48,63,113;,
    3;65,61,116;,
    3;64,65,116;,
    3;109,117,63;,
    3;48,109,63;,
    3;66,118,63;,
    3;117,66,63;,
    3;68,118,66;,
    3;67,68,66;,
    3;43,69,119;,
    3;45,43,119;,
    3;70,120,119;,
    3;69,70,119;,
    3;96,122,121;,
    3;40,71,70;,
    3;69,40,70;,
    3;123,96,121;,
    3;123,124,96;,
    3;72,125,96;,
    3;125,122,96;,
    3;96,73,72;,
    3;74,65,126;,
    3;127,74,126;,
    3;64,128,126;,
    3;65,64,126;,
    3;75,76,74;,
    3;127,75,74;,
    3;77,62,74;,
    3;76,77,74;,
    3;61,65,74;,
    3;62,61,74;,
    3;78,79,77;,
    3;76,78,77;,
    3;80,58,77;,
    3;79,80,77;,
    3;57,62,77;,
    3;58,57,77;,
    3;81,82,80;,
    3;79,81,80;,
    3;83,84,80;,
    3;82,83,80;,
    3;59,58,80;,
    3;84,59,80;,
    3;1,3,84;,
    3;83,1,84;,
    3;53,59,84;,
    3;3,53,84;,
    3;85,86,83;,
    3;82,85,83;,
    3;2,1,83;,
    3;86,2,83;,
    3;87,88,129;,
    3;130,87,129;,
    3;89,131,129;,
    3;88,89,129;,
    3;90,91,89;,
    3;88,90,89;,
    3;90,132,92;,
    3;91,90,92;,
    3;11,9,133;,
    3;18,11,133;,
    3;134,132,90;,
    3;135,134,90;,
    3;87,135,90;,
    3;88,87,90;,
    3;99,135,87;,
    3;28,99,87;,
    3;93,28,87;,
    3;130,93,87;,
    3;29,28,93;,
    3;31,29,93;,
    3;82,81,136;,
    3;85,82,136;,
    3;94,137,136;,
    3;81,94,136;,
    3;138,137,94;,
    3;139,138,94;,
    3;79,78,94;,
    3;81,79,94;,
    3;95,139,94;,
    3;78,95,94;,
    3;96,33,140;,
    3;76,75,95;,
    3;78,76,95;,
    3;75,141,95;,
    3;75,127,141;,
    3;124,35,96;,
    3;38,37,71;,
    3;40,38,71;,
    3;41,40,69;,
    3;43,41,69;,
    3;143,97,142;,
    3;145,97,144;,
    3;147,97,146;,
    3;149,97,148;,
    3;151,97,150;,
    3;142,97,152;,
    3;154,97,153;,
    3;153,97,155;,
    3;155,97,145;,
    3;146,97,154;,
    3;148,97,156;,
    3;156,97,147;,
    3;152,97,149;,
    3;150,97,143;,
    3;157,97,151;,
    3;144,97,157;;
    MeshNormals
    {
     158;
     -0.707591;0.522885;-0.475296;,
     -0.436137;-0.738793;0.513779;,
     -0.470423;-0.466736;0.748905;,
     -0.425576;-0.869396;0.251070;,
     0.133083;0.816496;-0.561804;,
     0.177915;0.559822;-0.809287;,
     -0.594392;0.168948;-0.786228;,
     -0.683642;0.727412;-0.059207;,
     0.127899;0.962651;-0.238631;,
     -0.608501;0.629207;0.483554;,
     -0.644728;0.733906;0.213792;,
     0.182614;0.868996;0.459890;,
     0.157537;0.981192;0.111554;,
     0.317327;0.942434;0.105457;,
     0.308741;0.920659;-0.238887;,
     0.305923;0.847533;0.433705;,
     0.226384;0.893499;0.387826;,
     0.253528;0.963747;0.083162;,
     0.182794;0.627203;0.757101;,
     0.260322;0.640040;0.722898;,
     0.178569;0.704530;0.686841;,
     0.202956;0.767728;0.607784;,
     0.194381;0.934871;0.297040;,
     0.200739;0.310567;0.929113;,
     0.144653;0.313715;0.938434;,
     0.209471;0.304338;0.929247;,
     0.562272;0.242660;0.790548;,
     0.524342;0.717803;0.458066;,
     0.139943;-0.183674;0.972975;,
     0.177364;-0.259567;0.949298;,
     0.469410;-0.300462;0.830287;,
     0.140211;-0.552816;0.821423;,
     0.410920;-0.530095;0.741717;,
     0.370114;-0.730323;0.574147;,
     0.475645;0.859605;0.186656;,
     0.439909;0.898042;-0.000237;,
     0.178182;0.982998;0.044341;,
     0.419247;0.889515;-0.181645;,
     0.140723;0.970808;-0.194238;,
     0.239047;0.943441;-0.229731;,
     0.084108;0.879097;-0.469163;,
     0.179582;0.835452;-0.519394;,
     0.289043;0.788847;-0.542379;,
     0.102771;0.659945;-0.744252;,
     0.254239;0.590918;-0.765623;,
     0.022226;0.336130;-0.941553;,
     0.191522;0.293479;-0.936584;,
     0.214718;0.224835;-0.950445;,
     0.111047;-0.163058;-0.980347;,
     0.211547;-0.216873;-0.953003;,
     -0.467479;-0.368853;-0.803375;,
     -0.514225;-0.075824;-0.854297;,
     -0.461314;-0.832971;-0.305531;,
     -0.441450;-0.897240;0.009077;,
     -0.457710;-0.645687;-0.611220;,
     0.151945;-0.651611;-0.743180;,
     0.080283;-0.907832;-0.411577;,
     0.041200;-0.903790;-0.425988;,
     0.053172;-0.992766;-0.107652;,
     0.044765;-0.994865;-0.090771;,
     0.052252;-0.633604;-0.771891;,
     -0.036011;-0.676617;-0.735454;,
     -0.005841;-0.916523;-0.399940;,
     -0.034888;-0.184057;-0.982296;,
     0.024378;-0.285349;-0.958114;,
     0.043933;-0.734715;-0.676952;,
     0.020807;0.300582;-0.953529;,
     0.440192;0.209589;-0.873100;,
     0.413622;-0.353212;-0.839141;,
     0.041307;0.688753;-0.723819;,
     0.417136;0.610409;-0.673349;,
     0.410762;0.800831;-0.435825;,
     0.358753;-0.726271;-0.586368;,
     0.321063;-0.877772;-0.355577;,
     0.053284;-0.920752;-0.386493;,
     0.313525;-0.939296;-0.139375;,
     0.052102;-0.989884;-0.131962;,
     0.017583;-0.993231;-0.114819;,
     0.064343;-0.964369;0.256617;,
     0.057861;-0.965970;0.252100;,
     0.099067;-0.966843;0.235371;,
     0.115824;-0.789299;0.602986;,
     0.164301;-0.790866;0.589522;,
     0.096874;-0.787205;0.609035;,
     0.060479;-0.971039;0.231140;,
     0.180944;-0.489559;0.852990;,
     0.101242;-0.444106;0.890236;,
     0.171938;-0.100354;0.979983;,
     0.107527;-0.074568;0.991402;,
     -0.501760;-0.179872;0.846099;,
     0.148916;0.292037;0.944743;,
     -0.530703;0.102940;0.841284;,
     -0.570318;0.405999;0.714074;,
     0.144490;-0.540513;0.828836;,
     0.098802;-0.774588;0.624701;,
     0.334379;-0.915591;0.223347;,
     0.996941;-0.069164;-0.036396;,
     -0.960433;-0.263498;0.090211;,
     0.209471;0.304338;0.929247;,
     0.144653;0.313715;0.938434;,
     0.562272;0.242660;0.790548;,
     0.469410;-0.300462;0.830287;,
     0.562272;0.242660;0.790548;,
     0.410920;-0.530095;0.741717;,
     0.524342;0.717803;0.458066;,
     0.475645;0.859605;0.186656;,
     0.439909;0.898042;-0.000237;,
     0.254239;0.590918;-0.765623;,
     0.177915;0.559822;-0.809287;,
     0.191522;0.293479;-0.936584;,
     -0.594392;0.168948;-0.786228;,
     0.211547;-0.216873;-0.953003;,
     -0.467479;-0.368853;-0.803375;,
     0.052252;-0.633604;-0.771891;,
     0.151945;-0.651611;-0.743180;,
     -0.036011;-0.676617;-0.735454;,
     -0.034888;-0.184057;-0.982296;,
     0.022226;0.336130;-0.941553;,
     0.024378;-0.285349;-0.958114;,
     0.020807;0.300582;-0.953529;,
     0.440192;0.209589;-0.873100;,
     0.417136;0.610409;-0.673349;,
     0.440192;0.209589;-0.873100;,
     0.410762;0.800831;-0.435825;,
     0.419247;0.889515;-0.181645;,
     0.413622;-0.353212;-0.839141;,
     0.358753;-0.726271;-0.586368;,
     0.321063;-0.877772;-0.355577;,
     0.413622;-0.353212;-0.839141;,
     0.101242;-0.444106;0.890236;,
     0.180944;-0.489559;0.852990;,
     -0.470423;-0.466736;0.748905;,
     0.182794;0.627203;0.757101;,
     -0.570318;0.405999;0.714074;,
     0.260322;0.640040;0.722898;,
     0.200739;0.310567;0.929113;,
     0.144490;-0.540513;0.828836;,
     0.140211;-0.552816;0.821423;,
     0.410920;-0.530095;0.741717;,
     0.370114;-0.730323;0.574147;,
     0.334379;-0.915591;0.223347;,
     0.996941;-0.069164;-0.036396;,
     -0.436137;-0.738793;0.513779;,
     -0.470423;-0.466736;0.748905;,
     -0.608501;0.629207;0.483554;,
     -0.644728;0.733906;0.213792;,
     -0.514225;-0.075824;-0.854297;,
     -0.467479;-0.368853;-0.803375;,
     -0.461314;-0.832971;-0.305531;,
     -0.441450;-0.897240;0.009077;,
     -0.501760;-0.179872;0.846099;,
     -0.530703;0.102940;0.841284;,
     -0.425576;-0.869396;0.251070;,
     -0.707591;0.522885;-0.475296;,
     -0.594392;0.168948;-0.786228;,
     -0.683642;0.727412;-0.059207;,
     -0.457710;-0.645687;-0.611220;,
     -0.570318;0.405999;0.714074;;
     192;
     3;5,6,0;,
     3;4,5,0;,
     3;8,4,0;,
     3;7,8,0;,
     3;11,12,10;,
     3;9,11,10;,
     3;8,7,10;,
     3;12,8,10;,
     3;14,8,12;,
     3;13,14,12;,
     3;15,13,12;,
     3;11,15,12;,
     3;17,13,15;,
     3;16,17,15;,
     3;18,19,15;,
     3;11,18,15;,
     3;20,16,15;,
     3;19,20,15;,
     3;22,16,20;,
     3;21,22,20;,
     3;23,24,20;,
     3;19,23,20;,
     3;25,21,20;,
     3;24,25,20;,
     3;27,21,25;,
     3;26,27,25;,
     3;28,29,98;,
     3;99,28,98;,
     3;30,100,98;,
     3;29,30,98;,
     3;96,102,101;,
     3;31,32,30;,
     3;29,31,30;,
     3;103,96,101;,
     3;103,33,96;,
     3;34,104,96;,
     3;104,102,96;,
     3;96,35,34;,
     3;36,22,105;,
     3;106,36,105;,
     3;21,27,105;,
     3;22,21,105;,
     3;37,38,36;,
     3;106,37,36;,
     3;39,17,36;,
     3;38,39,36;,
     3;16,22,36;,
     3;17,16,36;,
     3;40,41,39;,
     3;38,40,39;,
     3;42,14,39;,
     3;41,42,39;,
     3;13,17,39;,
     3;14,13,39;,
     3;43,44,42;,
     3;41,43,42;,
     3;5,4,42;,
     3;44,5,42;,
     3;8,14,42;,
     3;4,8,42;,
     3;45,46,44;,
     3;43,45,44;,
     3;47,108,107;,
     3;109,47,107;,
     3;48,49,47;,
     3;109,48,47;,
     3;50,51,47;,
     3;49,50,47;,
     3;110,108,47;,
     3;51,110,47;,
     3;56,52,54;,
     3;55,56,54;,
     3;111,55,54;,
     3;112,111,54;,
     3;58,59,56;,
     3;57,58,56;,
     3;53,52,56;,
     3;59,53,56;,
     3;60,57,56;,
     3;55,60,56;,
     3;62,57,60;,
     3;61,62,60;,
     3;49,48,113;,
     3;114,49,113;,
     3;63,115,113;,
     3;48,63,113;,
     3;65,61,116;,
     3;64,65,116;,
     3;109,117,63;,
     3;48,109,63;,
     3;66,118,63;,
     3;117,66,63;,
     3;68,118,66;,
     3;67,68,66;,
     3;43,69,119;,
     3;45,43,119;,
     3;70,120,119;,
     3;69,70,119;,
     3;96,122,121;,
     3;40,71,70;,
     3;69,40,70;,
     3;123,96,121;,
     3;123,124,96;,
     3;72,125,96;,
     3;125,122,96;,
     3;96,73,72;,
     3;74,65,126;,
     3;127,74,126;,
     3;64,128,126;,
     3;65,64,126;,
     3;75,76,74;,
     3;127,75,74;,
     3;77,62,74;,
     3;76,77,74;,
     3;61,65,74;,
     3;62,61,74;,
     3;78,79,77;,
     3;76,78,77;,
     3;80,58,77;,
     3;79,80,77;,
     3;57,62,77;,
     3;58,57,77;,
     3;81,82,80;,
     3;79,81,80;,
     3;83,84,80;,
     3;82,83,80;,
     3;59,58,80;,
     3;84,59,80;,
     3;1,3,84;,
     3;83,1,84;,
     3;53,59,84;,
     3;3,53,84;,
     3;85,86,83;,
     3;82,85,83;,
     3;2,1,83;,
     3;86,2,83;,
     3;87,88,129;,
     3;130,87,129;,
     3;89,131,129;,
     3;88,89,129;,
     3;90,91,89;,
     3;88,90,89;,
     3;90,132,92;,
     3;91,90,92;,
     3;11,9,133;,
     3;18,11,133;,
     3;134,132,90;,
     3;135,134,90;,
     3;87,135,90;,
     3;88,87,90;,
     3;99,135,87;,
     3;28,99,87;,
     3;93,28,87;,
     3;130,93,87;,
     3;29,28,93;,
     3;31,29,93;,
     3;82,81,136;,
     3;85,82,136;,
     3;94,137,136;,
     3;81,94,136;,
     3;138,137,94;,
     3;139,138,94;,
     3;79,78,94;,
     3;81,79,94;,
     3;95,139,94;,
     3;78,95,94;,
     3;96,33,140;,
     3;76,75,95;,
     3;78,76,95;,
     3;75,141,95;,
     3;75,127,141;,
     3;124,35,96;,
     3;38,37,71;,
     3;40,38,71;,
     3;41,40,69;,
     3;43,41,69;,
     3;143,97,142;,
     3;145,97,144;,
     3;147,97,146;,
     3;149,97,148;,
     3;151,97,150;,
     3;142,97,152;,
     3;154,97,153;,
     3;153,97,155;,
     3;155,97,145;,
     3;146,97,154;,
     3;148,97,156;,
     3;156,97,147;,
     3;152,97,149;,
     3;150,97,143;,
     3;157,97,151;,
     3;144,97,157;;
    }
    MeshTextureCoords
    {
     158;
     0.374913;0.417303;,
     0.033542;0.422964;,
     0.001992;0.410250;,
     0.094913;0.423499;,
     0.392831;0.544943;,
     0.346595;0.552677;,
     0.321815;0.453091;,
     0.459272;0.432515;,
     0.453497;0.548006;,
     0.572329;0.454778;,
     0.520134;0.443841;,
     0.566429;0.567790;,
     0.514252;0.557739;,
     0.510834;0.648468;,
     0.458787;0.643364;,
     0.556166;0.650577;,
     0.551470;0.733760;,
     0.508425;0.735211;,
     0.605894;0.573150;,
     0.593157;0.649248;,
     0.586572;0.730405;,
     0.571524;0.869371;,
     0.536959;0.868925;,
     0.617280;0.645450;,
     0.609792;0.727744;,
     0.594475;0.868930;,
     0.559438;0.998259;,
     0.535411;0.998054;,
     0.740321;0.725985;,
     0.748089;0.864311;,
     0.754216;0.992665;,
     0.785633;0.856409;,
     0.785652;0.985937;,
     0.575781;0.375555;,
     0.703725;0.357824;,
     0.708844;0.321200;,
     0.494035;0.866397;,
     0.426189;0.985748;,
     0.444007;0.860654;,
     0.458030;0.731029;,
     0.395223;0.851182;,
     0.409010;0.721648;,
     0.407090;0.637306;,
     0.370962;0.712925;,
     0.367300;0.633385;,
     0.343681;0.704991;,
     0.343022;0.629273;,
     0.904948;0.576623;,
     0.847066;0.643283;,
     0.849425;0.582458;,
     0.831184;0.505692;,
     0.875788;0.492711;,
     0.233933;0.401182;,
     0.167926;0.408719;,
     0.285566;0.402853;,
     0.279616;0.474497;,
     0.233624;0.473173;,
     0.227951;0.534164;,
     0.173186;0.536418;,
     0.170264;0.480648;,
     0.269890;0.534212;,
     0.269703;0.608995;,
     0.229294;0.610809;,
     0.834368;0.716977;,
     0.296609;0.747396;,
     0.272128;0.748439;,
     0.866988;0.856224;,
     0.860891;0.998259;,
     0.829470;0.997998;,
     0.355637;0.842751;,
     0.360102;0.981093;,
     0.388137;0.982121;,
     0.587350;0.223162;,
     0.569701;0.256779;,
     0.233013;0.748821;,
     0.182836;0.885789;,
     0.184800;0.748304;,
     0.179339;0.611651;,
     0.130978;0.749347;,
     0.120577;0.616369;,
     0.106977;0.546540;,
     0.069075;0.626166;,
     0.052057;0.556402;,
     0.039141;0.496805;,
     0.097699;0.491724;,
     0.016532;0.556734;,
     0.005219;0.493213;,
     0.737983;0.645050;,
     0.730004;0.577723;,
     0.725938;0.490262;,
     0.673810;0.575989;,
     0.668852;0.477037;,
     0.623903;0.464976;,
     0.780256;0.724179;,
     0.085011;0.755834;,
     0.139823;0.886065;,
     0.620552;0.307799;,
     0.833993;0.283493;,
     0.710145;0.868930;,
     0.700780;0.727744;,
     0.721525;0.998259;,
     0.636439;0.410788;,
     0.669130;0.413311;,
     0.605003;0.397051;,
     0.691394;0.389285;,
     0.503951;0.995062;,
     0.467327;0.990388;,
     0.939253;0.650249;,
     0.956217;0.569541;,
     0.895047;0.646137;,
     0.927773;0.469955;,
     0.305497;0.474343;,
     0.315675;0.397578;,
     0.799893;0.642327;,
     0.799702;0.582611;,
     0.793462;0.717109;,
     0.289740;0.608863;,
     0.879227;0.721855;,
     0.827573;0.855510;,
     0.331644;0.839360;,
     0.341858;0.981395;,
     0.674948;0.213975;,
     0.645776;0.195732;,
     0.697574;0.242011;,
     0.708493;0.280062;,
     0.614355;0.200383;,
     0.257728;0.888962;,
     0.224111;0.887294;,
     0.280507;0.889884;,
     0.784161;0.584464;,
     0.783763;0.647984;,
     0.782980;0.501500;,
     0.626219;0.573150;,
     0.612076;0.464976;,
     0.649859;0.649248;,
     0.690596;0.645450;,
     0.033884;0.632928;,
     0.053596;0.765159;,
     0.083839;0.894687;,
     0.105334;0.889016;,
     0.556319;0.341066;,
     0.173091;0.998259;,
     0.745746;0.173410;,
     0.790237;0.141860;,
     0.982040;0.194555;,
     0.998008;0.246749;,
     0.843235;0.460984;,
     0.798632;0.455542;,
     0.727041;0.373801;,
     0.714980;0.307793;,
     0.847279;0.127986;,
     0.904365;0.132152;,
     0.719515;0.234780;,
     0.951623;0.391971;,
     0.895220;0.445069;,
     0.992705;0.307611;,
     0.755892;0.425434;,
     0.949314;0.154808;;
    }
    MeshMaterialList
    {
     1;
     192;
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
     158;
     98;
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
     89,
     90,
     91,
     92,
     93,
     94,
     95,
     96,
     97,
     25,
     24,
     26,
     30,
     26,
     32,
     27,
     34,
     35,
     44,
     5,
     46,
     6,
     49,
     50,
     60,
     55,
     61,
     63,
     45,
     64,
     66,
     67,
     70,
     67,
     71,
     37,
     68,
     72,
     73,
     68,
     86,
     85,
     2,
     18,
     92,
     19,
     23,
     93,
     31,
     32,
     33,
     95,
     96,
     1,
     2,
     9,
     10,
     51,
     50,
     52,
     53,
     89,
     91,
     3,
     0,
     6,
     7,
     54,
     92;
    }
   }
  }
 }
}

# Animations definition.

