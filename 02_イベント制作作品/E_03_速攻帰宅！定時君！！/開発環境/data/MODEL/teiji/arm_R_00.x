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
  "data/TEXTURE/color_001.jpg";
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
 Frame Mesh_1
 {
  FrameTransformMatrix
  {
   1.000000,0.000000,0.000000,0.000000,
   0.000000,1.000000,0.000000,0.000000,
   0.000000,0.000000,1.000000,0.000000,
   0.000000,0.000000,0.000000,1.000000;;
  }
  Mesh
  {
   84;
   5.547421;-12.943760;-1.772834;,
   3.896962;-13.402964;-1.807034;,
   5.576435;-13.283349;1.256246;,
   3.925979;-13.742547;1.222046;,
   2.826925;-12.886420;1.710850;,
   5.835493;-11.410745;1.056151;,
   3.324055;-13.973855;-0.334656;,
   6.296514;-13.165211;-0.274078;,
   2.750054;-11.881899;-1.816648;,
   5.496367;-10.903826;-1.363465;,
   2.463807;-12.453282;-0.376401;,
   6.021479;-11.046844;-0.269024;,
   4.713565;-11.294276;-2.298779;,
   3.678022;-11.713616;-2.402256;,
   3.744687;-12.601475;2.124133;,
   4.808022;-12.177983;2.147661;,
   5.459537;-13.891315;-0.336248;,
   4.360440;-14.210725;-0.359776;,
   3.476826;-10.962219;2.204527;,
   5.196902;-10.397367;1.533484;,
   3.487848;-8.302572;2.060430;,
   4.778426;-7.818957;2.225266;,
   4.533978;-5.672678;2.250918;,
   3.535830;-6.807759;2.155058;,
   2.462358;-3.955137;2.685988;,
   3.608043;-3.729031;2.464137;,
   4.470850;-4.196944;0.987078;,
   5.422754;-7.323489;0.726538;,
   4.098750;-3.266138;-0.297386;,
   3.472203;-1.427527;-0.362434;,
   3.480242;-1.387695;1.189155;,
   3.140187;-2.098238;2.468411;,
   1.146829;0.316040;2.108792;,
   1.346347;-2.038872;2.708570;,
   1.014404;-0.949515;2.709745;,
   3.024395;-1.710156;-1.832525;,
   2.971770;-3.707684;-2.578548;,
   3.876453;-4.055276;-1.526539;,
   1.875115;-4.537904;-2.618058;,
   4.562804;-6.455566;-1.916474;,
   4.034300;-6.847252;-2.821790;,
   5.778189;-10.000364;-0.343904;,
   2.870268;-7.411677;-2.752427;,
   1.988454;-11.094151;-0.364003;,
   1.001452;-5.297369;-1.683457;,
   1.480871;-7.648743;-1.968804;,
   1.244282;-7.627935;-0.363484;,
   0.807257;-5.560060;-0.072488;,
   0.536153;-3.426263;-2.342973;,
   0.468338;-4.160860;-0.427143;,
   2.347594;-11.426417;1.583472;,
   2.213094;-8.768376;1.512479;,
   2.090990;-7.339277;1.590280;,
   1.330823;-4.994052;2.001243;,
   0.566364;-3.590958;1.840861;,
   0.714793;-1.698740;-2.922297;,
   0.413601;0.062506;-2.872242;,
   2.542381;-0.443808;-2.188179;,
   1.505491;1.098025;-1.925923;,
   2.769561;1.479706;-1.127328;,
   2.914269;0.348755;-0.981606;,
   1.361086;1.731778;0.072576;,
   2.799831;1.481928;0.504422;,
   1.754219;1.227475;1.424930;,
   2.849891;0.408487;0.729363;,
   4.523416;-4.788493;-0.202716;,
   5.066307;-6.571943;-0.663643;,
   5.531013;-8.342927;-0.418760;,
   0.332549;1.336691;-2.119722;,
   0.341788;1.668289;-1.221274;,
   0.182029;-1.727142;3.703002;,
   0.280970;-0.400662;3.010350;,
   0.331656;1.678043;1.140048;,
   0.349693;1.837140;0.076692;,
   -0.489379;-2.962833;1.538141;,
   -0.511922;-3.260457;-0.304674;,
   -0.397379;-2.755447;-1.921936;,
   -0.203899;-1.557504;-2.255836;,
   -0.475023;0.424539;-1.735867;,
   -0.629478;-1.580191;3.036540;,
   -0.550444;-0.520584;2.483241;,
   -0.533007;0.391705;1.752445;,
   -0.502750;1.203458;0.564508;,
   -1.497306;-0.892038;0.086595;;
   164;
   3;10,8,1;,
   3;6,10,1;,
   3;4,10,6;,
   3;3,4,6;,
   3;11,5,2;,
   3;7,11,2;,
   3;9,11,7;,
   3;0,9,7;,
   3;16,17,1;,
   3;0,16,1;,
   3;13,12,0;,
   3;1,13,0;,
   3;17,16,2;,
   3;3,17,2;,
   3;15,14,3;,
   3;2,15,3;,
   3;12,9,0;,
   3;8,13,1;,
   3;14,4,3;,
   3;15,2,5;,
   3;16,7,2;,
   3;6,17,3;,
   3;6,1,17;,
   3;16,0,7;,
   3;18,14,15;,
   3;19,18,15;,
   3;20,18,19;,
   3;21,20,19;,
   3;21,22,20;,
   3;22,23,20;,
   3;24,23,22;,
   3;25,24,22;,
   3;26,22,21;,
   3;27,26,21;,
   3;26,25,22;,
   3;28,29,26;,
   3;29,30,26;,
   3;26,30,31;,
   3;31,32,24;,
   3;32,31,30;,
   3;32,33,24;,
   3;33,32,34;,
   3;25,26,31;,
   3;29,28,35;,
   3;35,37,36;,
   3;36,37,39;,
   3;40,36,39;,
   3;38,36,40;,
   3;39,9,40;,
   3;9,12,40;,
   3;41,11,9;,
   3;42,40,12;,
   3;13,42,12;,
   3;42,38,40;,
   3;43,8,10;,
   3;44,38,42;,
   3;45,44,42;,
   3;45,46,44;,
   3;46,47,44;,
   3;46,45,8;,
   3;43,46,8;,
   3;8,45,42;,
   3;13,8,42;,
   3;44,47,48;,
   3;44,48,38;,
   3;47,49,48;,
   3;50,10,4;,
   3;43,10,50;,
   3;51,43,50;,
   3;46,43,51;,
   3;52,46,51;,
   3;46,52,53;,
   3;47,46,53;,
   3;47,53,54;,
   3;49,47,54;,
   3;50,4,14;,
   3;18,50,14;,
   3;51,50,18;,
   3;20,51,18;,
   3;52,51,20;,
   3;23,52,20;,
   3;53,52,23;,
   3;24,53,23;,
   3;54,53,24;,
   3;33,54,24;,
   3;48,55,38;,
   3;55,36,38;,
   3;35,36,55;,
   3;57,55,56;,
   3;35,55,57;,
   3;58,57,56;,
   3;60,57,58;,
   3;59,60,58;,
   3;59,58,61;,
   3;62,59,61;,
   3;62,61,63;,
   3;35,57,60;,
   3;29,35,60;,
   3;29,60,64;,
   3;64,60,59;,
   3;62,64,59;,
   3;64,62,63;,
   3;32,64,63;,
   3;30,29,64;,
   3;64,32,30;,
   3;26,65,28;,
   3;35,28,65;,
   3;37,35,65;,
   3;65,66,37;,
   3;66,39,37;,
   3;66,67,39;,
   3;65,26,27;,
   3;66,65,27;,
   3;66,27,19;,
   3;27,21,19;,
   3;67,66,19;,
   3;5,19,15;,
   3;5,11,19;,
   3;11,41,19;,
   3;67,9,39;,
   3;67,41,9;,
   3;41,67,19;,
   3;31,24,25;,
   3;56,68,58;,
   3;68,69,58;,
   3;54,33,70;,
   3;70,33,34;,
   3;71,70,34;,
   3;71,34,32;,
   3;63,72,32;,
   3;63,61,72;,
   3;61,73,72;,
   3;61,58,73;,
   3;58,69,73;,
   3;74,75,49;,
   3;54,74,49;,
   3;75,76,48;,
   3;49,75,48;,
   3;76,77,55;,
   3;48,76,55;,
   3;77,78,56;,
   3;55,77,56;,
   3;56,78,68;,
   3;68,78,69;,
   3;79,74,54;,
   3;70,79,54;,
   3;80,79,70;,
   3;71,80,70;,
   3;81,80,71;,
   3;32,81,71;,
   3;82,81,32;,
   3;72,82,32;,
   3;73,82,72;,
   3;78,82,73;,
   3;69,78,73;,
   3;74,83,75;,
   3;75,83,76;,
   3;76,83,77;,
   3;77,83,78;,
   3;79,83,74;,
   3;80,83,79;,
   3;81,83,80;,
   3;82,83,81;,
   3;78,83,82;;
   MeshNormals
   {
    84;
    0.618970;-0.350667;-0.702786;,
    -0.237567;-0.647543;-0.724051;,
    0.648302;-0.531685;0.544991;,
    -0.144646;-0.842070;0.519611;,
    -0.650164;-0.489281;0.581284;,
    0.855926;0.130059;0.500475;,
    -0.526865;-0.841309;-0.120880;,
    0.917208;-0.386935;-0.094923;,
    -0.756983;-0.304176;-0.578320;,
    0.857546;0.123233;-0.499427;,
    -0.892364;-0.446392;-0.066491;,
    0.978459;0.206413;0.003438;,
    0.379380;-0.054040;-0.923661;,
    -0.183427;-0.221643;-0.957721;,
    -0.174276;-0.303721;0.936687;,
    0.414483;-0.065514;0.907696;,
    0.481380;-0.870634;-0.101341;,
    0.019713;-0.993659;-0.110698;,
    -0.127258;0.034573;0.991267;,
    0.796096;0.134966;0.589929;,
    -0.140117;-0.056378;0.988529;,
    0.672346;0.022896;0.739883;,
    0.406924;0.045086;0.912349;,
    -0.175232;-0.162211;0.971072;,
    -0.036144;-0.054946;0.997835;,
    0.592094;0.182458;0.784942;,
    0.944929;0.280055;0.169349;,
    0.985339;0.153573;0.074310;,
    0.935558;0.278876;-0.216703;,
    0.947406;0.298000;-0.116700;,
    0.846357;0.410714;0.339109;,
    0.581790;0.298746;0.756486;,
    0.322471;0.440795;0.837683;,
    0.442384;-0.041932;0.895845;,
    0.634435;0.272980;0.723170;,
    0.810906;0.216660;-0.543591;,
    0.507867;0.205750;-0.836503;,
    0.862610;0.295754;-0.410406;,
    -0.268131;-0.143628;-0.952616;,
    0.855319;0.229245;-0.464624;,
    0.429125;0.080811;-0.899623;,
    0.982263;0.184962;-0.030796;,
    -0.306733;-0.099901;-0.946538;,
    -0.974107;-0.226000;-0.006255;,
    -0.823756;-0.225491;-0.520173;,
    -0.805418;-0.177839;-0.565398;,
    -0.969810;-0.226932;0.089278;,
    -0.959991;-0.275965;0.047556;,
    -0.734594;-0.383334;-0.559845;,
    -0.851581;-0.522854;-0.037853;,
    -0.795704;-0.160200;0.584115;,
    -0.742602;-0.068151;0.666256;,
    -0.593725;-0.179217;0.784456;,
    -0.656946;-0.303943;0.689957;,
    -0.586858;-0.524198;0.617101;,
    -0.066242;-0.034015;-0.997224;,
    -0.107348;0.308908;-0.945014;,
    0.568553;0.242662;-0.786043;,
    0.345509;0.730309;-0.589298;,
    0.742138;0.587284;-0.323000;,
    0.895750;0.240761;-0.373720;,
    0.101232;0.994510;0.026482;,
    0.610973;0.709350;0.351475;,
    0.358429;0.645071;0.674842;,
    0.899773;0.245247;0.360918;,
    0.935304;0.290389;-0.202190;,
    0.961382;0.226198;-0.156779;,
    0.978300;0.192404;-0.076880;,
    -0.301520;0.722179;-0.622530;,
    -0.274459;0.928098;-0.251608;,
    0.167839;-0.076011;0.982880;,
    0.083426;0.522963;0.848263;,
    -0.129223;0.873708;0.468972;,
    -0.164407;0.983832;-0.071024;,
    -0.787699;-0.558418;0.260191;,
    -0.810611;-0.583192;-0.052877;,
    -0.815829;-0.331821;-0.473621;,
    -0.768032;-0.043394;-0.638940;,
    -0.829811;0.361164;-0.425412;,
    -0.848159;-0.085329;0.522824;,
    -0.714605;0.440792;0.543178;,
    -0.678851;0.515982;0.522421;,
    -0.761708;0.626058;0.166888;,
    -0.999163;0.035862;-0.019685;;
    164;
    3;10,8,1;,
    3;6,10,1;,
    3;4,10,6;,
    3;3,4,6;,
    3;11,5,2;,
    3;7,11,2;,
    3;9,11,7;,
    3;0,9,7;,
    3;16,17,1;,
    3;0,16,1;,
    3;13,12,0;,
    3;1,13,0;,
    3;17,16,2;,
    3;3,17,2;,
    3;15,14,3;,
    3;2,15,3;,
    3;12,9,0;,
    3;8,13,1;,
    3;14,4,3;,
    3;15,2,5;,
    3;16,7,2;,
    3;6,17,3;,
    3;6,1,17;,
    3;16,0,7;,
    3;18,14,15;,
    3;19,18,15;,
    3;20,18,19;,
    3;21,20,19;,
    3;21,22,20;,
    3;22,23,20;,
    3;24,23,22;,
    3;25,24,22;,
    3;26,22,21;,
    3;27,26,21;,
    3;26,25,22;,
    3;28,29,26;,
    3;29,30,26;,
    3;26,30,31;,
    3;31,32,24;,
    3;32,31,30;,
    3;32,33,24;,
    3;33,32,34;,
    3;25,26,31;,
    3;29,28,35;,
    3;35,37,36;,
    3;36,37,39;,
    3;40,36,39;,
    3;38,36,40;,
    3;39,9,40;,
    3;9,12,40;,
    3;41,11,9;,
    3;42,40,12;,
    3;13,42,12;,
    3;42,38,40;,
    3;43,8,10;,
    3;44,38,42;,
    3;45,44,42;,
    3;45,46,44;,
    3;46,47,44;,
    3;46,45,8;,
    3;43,46,8;,
    3;8,45,42;,
    3;13,8,42;,
    3;44,47,48;,
    3;44,48,38;,
    3;47,49,48;,
    3;50,10,4;,
    3;43,10,50;,
    3;51,43,50;,
    3;46,43,51;,
    3;52,46,51;,
    3;46,52,53;,
    3;47,46,53;,
    3;47,53,54;,
    3;49,47,54;,
    3;50,4,14;,
    3;18,50,14;,
    3;51,50,18;,
    3;20,51,18;,
    3;52,51,20;,
    3;23,52,20;,
    3;53,52,23;,
    3;24,53,23;,
    3;54,53,24;,
    3;33,54,24;,
    3;48,55,38;,
    3;55,36,38;,
    3;35,36,55;,
    3;57,55,56;,
    3;35,55,57;,
    3;58,57,56;,
    3;60,57,58;,
    3;59,60,58;,
    3;59,58,61;,
    3;62,59,61;,
    3;62,61,63;,
    3;35,57,60;,
    3;29,35,60;,
    3;29,60,64;,
    3;64,60,59;,
    3;62,64,59;,
    3;64,62,63;,
    3;32,64,63;,
    3;30,29,64;,
    3;64,32,30;,
    3;26,65,28;,
    3;35,28,65;,
    3;37,35,65;,
    3;65,66,37;,
    3;66,39,37;,
    3;66,67,39;,
    3;65,26,27;,
    3;66,65,27;,
    3;66,27,19;,
    3;27,21,19;,
    3;67,66,19;,
    3;5,19,15;,
    3;5,11,19;,
    3;11,41,19;,
    3;67,9,39;,
    3;67,41,9;,
    3;41,67,19;,
    3;31,24,25;,
    3;56,68,58;,
    3;68,69,58;,
    3;54,33,70;,
    3;70,33,34;,
    3;71,70,34;,
    3;71,34,32;,
    3;63,72,32;,
    3;63,61,72;,
    3;61,73,72;,
    3;61,58,73;,
    3;58,69,73;,
    3;74,75,49;,
    3;54,74,49;,
    3;75,76,48;,
    3;49,75,48;,
    3;76,77,55;,
    3;48,76,55;,
    3;77,78,56;,
    3;55,77,56;,
    3;56,78,68;,
    3;68,78,69;,
    3;79,74,54;,
    3;70,79,54;,
    3;80,79,70;,
    3;71,80,70;,
    3;81,80,71;,
    3;32,81,71;,
    3;82,81,32;,
    3;72,82,32;,
    3;73,82,72;,
    3;78,82,73;,
    3;69,78,73;,
    3;74,83,75;,
    3;75,83,76;,
    3;76,83,77;,
    3;77,83,78;,
    3;79,83,74;,
    3;80,83,79;,
    3;81,83,80;,
    3;82,83,81;,
    3;78,83,82;;
   }
   MeshTextureCoords
   {
    84;
    0.349803;0.151973;,
    0.349396;0.154228;,
    0.385839;0.153641;,
    0.385433;0.155897;,
    0.391248;0.151691;,
    0.383459;0.144444;,
    0.366913;0.157032;,
    0.367633;0.153061;,
    0.349281;0.146758;,
    0.354673;0.141954;,
    0.366416;0.149564;,
    0.367693;0.142656;,
    0.343545;0.143872;,
    0.342315;0.145931;,
    0.396165;0.150292;,
    0.396445;0.148212;,
    0.366894;0.156627;,
    0.366614;0.158196;,
    0.397121;0.142241;,
    0.389138;0.139466;,
    0.395407;0.129177;,
    0.397368;0.126802;,
    0.397673;0.116261;,
    0.396533;0.121835;,
    0.402849;0.107824;,
    0.400210;0.106714;,
    0.382637;0.109012;,
    0.379538;0.124368;,
    0.367356;0.104440;,
    0.366582;0.095410;,
    0.385041;0.095214;,
    0.400261;0.098704;,
    0.395982;0.086846;,
    0.403118;0.098413;,
    0.403132;0.093062;,
    0.349092;0.096798;,
    0.340217;0.106609;,
    0.352733;0.108316;,
    0.339747;0.110687;,
    0.348094;0.120106;,
    0.337323;0.122030;,
    0.366802;0.137516;,
    0.338148;0.124802;,
    0.366563;0.142889;,
    0.350866;0.114417;,
    0.347471;0.125966;,
    0.366569;0.125864;,
    0.370032;0.115707;,
    0.343020;0.105227;,
    0.365812;0.108835;,
    0.389733;0.144521;,
    0.388888;0.131465;,
    0.389813;0.124446;,
    0.394703;0.112927;,
    0.392795;0.106036;,
    0.336128;0.096742;,
    0.336723;0.088091;,
    0.344861;0.090578;,
    0.347981;0.083005;,
    0.357482;0.081131;,
    0.359216;0.086685;,
    0.371757;0.079893;,
    0.376895;0.081120;,
    0.387846;0.082370;,
    0.379571;0.086392;,
    0.368482;0.111918;,
    0.362999;0.120677;,
    0.365912;0.129376;,
    0.345676;0.081833;,
    0.356365;0.080205;,
    0.414948;0.096882;,
    0.406708;0.090366;,
    0.384457;0.080157;,
    0.371806;0.079375;,
    0.389193;0.102951;,
    0.367269;0.104412;,
    0.348029;0.101932;,
    0.344057;0.096048;,
    0.350243;0.086313;,
    0.407019;0.096160;,
    0.400437;0.090955;,
    0.391743;0.086475;,
    0.377610;0.082488;,
    0.371924;0.092780;;
   }
   MeshMaterialList
   {
    1;
    164;
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
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
    84;
    84;
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
    83;
   }
  }
 }
}

# Animations definition.

