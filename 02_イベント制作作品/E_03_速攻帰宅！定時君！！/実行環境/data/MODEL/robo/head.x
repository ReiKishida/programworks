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
 1.000000;1.000000;1.000000;1.000000;;
 100.000000;
 1.000000;1.000000;1.000000;;
 0.000000;0.000000;0.000000;;
 TextureFilename
 {
  "data/TEXTURE/robo.jpg";
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
   0.000000,-61.216541,-0.000000,1.000000;;
  }
  Mesh
  {
   229;
   3.972188;75.570175;-3.419090;,
   2.748840;77.104202;-3.419090;,
   0.981050;77.955521;-3.419090;,
   -0.981048;77.955521;-3.419090;,
   -2.748838;77.104202;-3.419090;,
   -3.972187;75.570175;-3.419090;,
   -4.408795;73.657265;-3.419090;,
   -3.972188;71.744370;-3.419090;,
   -2.748839;70.210335;-3.419090;,
   -0.981050;69.359009;-3.419090;,
   0.981049;69.359009;-3.419090;,
   2.748839;70.210335;-3.419097;,
   3.972188;71.744370;-3.419090;,
   4.408796;73.657265;-3.419090;,
   3.972188;75.570175;-13.373804;,
   2.748840;77.104202;-13.373804;,
   0.981050;77.955521;-13.373804;,
   -0.981048;77.955521;-13.373804;,
   -2.748838;77.104202;-13.373804;,
   -3.972187;75.570175;-13.373811;,
   -4.408795;73.657265;-13.373804;,
   -3.972188;71.744362;-13.373811;,
   -2.748839;70.210335;-13.373804;,
   -0.981050;69.359009;-13.373804;,
   0.981049;69.359009;-13.373811;,
   2.748839;70.210335;-13.373804;,
   3.972188;71.744362;-13.373811;,
   4.408796;73.657265;-13.373811;,
   2.937152;75.071716;-13.373804;,
   2.032573;76.206039;-13.373804;,
   0.725418;76.835518;-13.373804;,
   -0.725416;76.835518;-13.373804;,
   -2.032572;76.206039;-13.373804;,
   -2.937152;75.071732;-13.373811;,
   -3.259993;73.657265;-13.373804;,
   -2.937152;72.242821;-13.373811;,
   -2.032573;71.108513;-13.373804;,
   -0.725418;70.479004;-13.373804;,
   0.725417;70.479004;-13.373811;,
   2.032573;71.108513;-13.373804;,
   2.937152;72.242821;-13.373811;,
   3.259993;73.657265;-13.373811;,
   2.937152;75.071716;-11.748173;,
   2.032572;76.206039;-11.748173;,
   0.725418;76.835518;-11.748173;,
   -0.725416;76.835518;-11.748173;,
   -2.032572;76.206039;-11.748173;,
   -2.937152;75.071732;-11.748179;,
   -3.259993;73.657265;-11.748173;,
   -2.937152;72.242821;-11.748179;,
   -2.032573;71.108513;-11.748173;,
   -0.725417;70.479004;-11.748173;,
   0.725417;70.479004;-11.748179;,
   2.032572;71.108513;-11.748173;,
   2.937152;72.242821;-11.748179;,
   3.259993;73.657265;-11.748179;,
   2.319489;74.774269;-12.352753;,
   1.605135;75.670059;-12.352753;,
   0.572869;76.167160;-12.352753;,
   -0.572868;76.167160;-12.352753;,
   -1.605137;75.670059;-12.352753;,
   -2.319489;74.774284;-12.352753;,
   -2.574438;73.657265;-12.352753;,
   -2.319489;72.540268;-12.352753;,
   -1.605134;71.644501;-12.352753;,
   -0.572862;71.147392;-12.352753;,
   0.572862;71.147392;-12.352753;,
   1.605134;71.644501;-12.352753;,
   2.319490;72.540268;-12.352753;,
   2.574438;73.657265;-12.352753;,
   1.052953;74.164345;-12.942446;,
   0.728656;74.571022;-12.942446;,
   -0.000000;73.657265;-13.203300;,
   0.260075;74.796661;-12.942446;,
   -0.260077;74.796661;-12.942446;,
   -0.728680;74.571007;-12.942439;,
   -1.052951;74.164391;-12.942446;,
   -1.168693;73.657272;-12.942439;,
   -1.052959;73.150215;-12.942446;,
   -0.728658;72.743568;-12.942446;,
   -0.260042;72.517891;-12.942446;,
   0.260043;72.517891;-12.942446;,
   0.728658;72.743568;-12.942446;,
   1.052961;73.150215;-12.942446;,
   1.168692;73.657257;-12.942446;,
   -9.737849;61.216541;-8.736121;,
   9.737849;61.216541;-8.736121;,
   -9.737849;61.216541;8.736121;,
   9.737849;61.216541;8.736121;,
   -8.311916;81.344032;-8.736121;,
   -9.737849;77.505257;-8.736121;,
   -4.869411;82.934105;-8.736121;,
   -9.737849;77.505257;8.736121;,
   -8.311916;81.344032;8.736121;,
   -4.869411;82.934105;8.736121;,
   8.311916;81.344032;-8.736121;,
   4.869411;82.934105;-8.736121;,
   9.737849;77.505257;-8.736121;,
   9.737849;77.505257;8.736121;,
   4.869411;82.934105;8.736121;,
   8.311916;81.344032;8.736121;,
   -3.455252;81.772850;-0.790999;,
   -1.873257;81.772850;-0.790999;,
   -3.717190;97.755089;-1.052935;,
   -1.611320;97.755089;-1.052935;,
   -3.717190;97.755089;1.052935;,
   -1.611320;97.755089;1.052935;,
   -3.455252;81.772850;0.790999;,
   -1.873257;81.772850;0.790999;,
   -3.717190;95.534096;-1.052935;,
   -1.611320;95.534096;-1.052935;,
   -1.611320;95.534096;1.052935;,
   -3.717190;95.534096;1.052935;,
   -3.455252;95.534096;-0.790999;,
   -1.873257;95.534096;-0.790999;,
   -1.873257;95.534096;0.790999;,
   -3.455252;95.534096;0.790999;,
   -0.981048;77.955521;-3.419090;,
   -0.981048;77.955521;-13.373804;,
   3.972188;75.570175;-13.373804;,
   2.748840;77.104202;-13.373804;,
   2.748840;77.104202;-13.373804;,
   0.981050;77.955521;-13.373804;,
   2.032573;76.206039;-13.373804;,
   0.981050;77.955521;-13.373804;,
   -0.981048;77.955521;-13.373804;,
   0.725418;76.835518;-13.373804;,
   -0.981048;77.955521;-13.373804;,
   -2.748838;77.104202;-13.373804;,
   -0.725416;76.835518;-13.373804;,
   -2.748838;77.104202;-13.373804;,
   -3.972187;75.570175;-13.373811;,
   -2.032572;76.206039;-13.373804;,
   -3.972187;75.570175;-13.373811;,
   -4.408795;73.657265;-13.373804;,
   -2.937152;75.071732;-13.373811;,
   -4.408795;73.657265;-13.373804;,
   -3.972188;71.744362;-13.373811;,
   -3.259993;73.657265;-13.373804;,
   -3.972188;71.744362;-13.373811;,
   -2.748839;70.210335;-13.373804;,
   -2.937152;72.242821;-13.373811;,
   -2.748839;70.210335;-13.373804;,
   -0.981050;69.359009;-13.373804;,
   -2.032573;71.108513;-13.373804;,
   -0.981050;69.359009;-13.373804;,
   0.981049;69.359009;-13.373811;,
   -0.725418;70.479004;-13.373804;,
   0.981049;69.359009;-13.373811;,
   2.748839;70.210335;-13.373804;,
   0.725417;70.479004;-13.373811;,
   2.748839;70.210335;-13.373804;,
   3.972188;71.744362;-13.373811;,
   2.032573;71.108513;-13.373804;,
   3.972188;71.744362;-13.373811;,
   4.408796;73.657265;-13.373811;,
   2.937152;72.242821;-13.373811;,
   4.408796;73.657265;-13.373811;,
   3.972188;75.570175;-13.373804;,
   2.937152;75.071716;-13.373804;,
   3.259993;73.657265;-13.373811;,
   2.937152;75.071716;-13.373804;,
   2.032573;76.206039;-13.373804;,
   0.725418;76.835518;-13.373804;,
   -0.725416;76.835518;-13.373804;,
   -0.725416;76.835518;-13.373804;,
   -2.032572;76.206039;-13.373804;,
   -0.725416;76.835518;-11.748173;,
   -2.937152;75.071732;-13.373811;,
   -3.259993;73.657265;-13.373804;,
   -2.937152;72.242821;-13.373811;,
   -2.032573;71.108513;-13.373804;,
   -0.725418;70.479004;-13.373804;,
   0.725417;70.479004;-13.373811;,
   2.032573;71.108513;-13.373804;,
   2.937152;72.242821;-13.373811;,
   3.259993;73.657265;-13.373811;,
   2.937152;75.071716;-11.748173;,
   2.032572;76.206039;-11.748173;,
   0.725418;76.835518;-11.748173;,
   -0.725416;76.835518;-11.748173;,
   -2.032572;76.206039;-11.748173;,
   -2.937152;75.071732;-11.748179;,
   -3.259993;73.657265;-11.748173;,
   -2.937152;72.242821;-11.748179;,
   -2.032573;71.108513;-11.748173;,
   -0.725417;70.479004;-11.748173;,
   0.725417;70.479004;-11.748179;,
   2.032572;71.108513;-11.748173;,
   2.937152;72.242821;-11.748179;,
   3.259993;73.657265;-11.748179;,
   -8.311916;81.344032;-8.736121;,
   -8.311916;81.344032;8.736121;,
   8.311916;81.344032;-8.736121;,
   8.311916;81.344032;8.736121;,
   9.737849;77.505257;-8.736121;,
   -9.737849;77.505257;-8.736121;,
   -9.737849;61.216541;-8.736121;,
   9.737849;61.216541;-8.736121;,
   8.311916;81.344032;8.736121;,
   -8.311916;81.344032;8.736121;,
   -4.869411;82.934105;8.736121;,
   4.869411;82.934105;8.736121;,
   8.311916;81.344032;-8.736121;,
   4.869411;82.934105;-8.736121;,
   -4.869411;82.934105;-8.736121;,
   -8.311916;81.344032;-8.736121;,
   -9.737849;77.505257;-8.736121;,
   9.737849;77.505257;-8.736121;,
   9.737849;77.505257;8.736121;,
   9.737849;61.216541;8.736121;,
   -9.737849;61.216541;8.736121;,
   -9.737849;77.505257;8.736121;,
   9.737849;77.505257;8.736121;,
   -9.737849;77.505257;8.736121;,
   -3.717190;97.755089;-1.052935;,
   -1.611320;97.755089;-1.052935;,
   -3.717190;97.755089;1.052935;,
   -1.611320;97.755089;1.052935;,
   -1.611320;97.755089;1.052935;,
   -1.611320;97.755089;-1.052935;,
   -3.717190;95.534096;1.052935;,
   -3.717190;97.755089;-1.052935;,
   -3.717190;97.755089;1.052935;,
   -3.455252;95.534096;0.790999;,
   -1.873257;81.772850;-0.790999;,
   -1.873257;81.772850;0.790999;,
   -3.455252;81.772850;0.790999;,
   -3.455252;81.772850;-0.790999;;
   206;
   3;15,1,0;,
   3;14,15,0;,
   3;16,2,1;,
   3;15,16,1;,
   3;17,3,2;,
   3;16,17,2;,
   3;18,4,117;,
   3;118,18,117;,
   3;19,5,4;,
   3;18,19,4;,
   3;20,6,5;,
   3;19,20,5;,
   3;21,7,6;,
   3;20,21,6;,
   3;22,8,7;,
   3;21,22,7;,
   3;23,9,8;,
   3;22,23,8;,
   3;24,10,9;,
   3;23,24,9;,
   3;25,11,10;,
   3;24,25,10;,
   3;26,12,11;,
   3;25,26,11;,
   3;27,13,12;,
   3;26,27,12;,
   3;14,0,13;,
   3;27,14,13;,
   3;72,71,70;,
   3;72,73,71;,
   3;72,74,73;,
   3;72,75,74;,
   3;72,76,75;,
   3;72,77,76;,
   3;72,78,77;,
   3;72,79,78;,
   3;72,80,79;,
   3;72,81,80;,
   3;72,82,81;,
   3;72,83,82;,
   3;72,84,83;,
   3;72,70,84;,
   3;29,120,119;,
   3;28,29,119;,
   3;30,122,121;,
   3;123,30,121;,
   3;31,125,124;,
   3;126,31,124;,
   3;32,128,127;,
   3;129,32,127;,
   3;33,131,130;,
   3;132,33,130;,
   3;34,134,133;,
   3;135,34,133;,
   3;35,137,136;,
   3;138,35,136;,
   3;36,140,139;,
   3;141,36,139;,
   3;37,143,142;,
   3;144,37,142;,
   3;38,146,145;,
   3;147,38,145;,
   3;39,149,148;,
   3;150,39,148;,
   3;40,152,151;,
   3;153,40,151;,
   3;41,155,154;,
   3;156,41,154;,
   3;159,158,157;,
   3;160,159,157;,
   3;43,162,161;,
   3;42,43,161;,
   3;44,163,162;,
   3;43,44,162;,
   3;45,164,163;,
   3;44,45,163;,
   3;46,166,165;,
   3;167,46,165;,
   3;47,168,166;,
   3;46,47,166;,
   3;48,169,168;,
   3;47,48,168;,
   3;49,170,169;,
   3;48,49,169;,
   3;50,171,170;,
   3;49,50,170;,
   3;51,172,171;,
   3;50,51,171;,
   3;52,173,172;,
   3;51,52,172;,
   3;53,174,173;,
   3;52,53,173;,
   3;54,175,174;,
   3;53,54,174;,
   3;55,176,175;,
   3;54,55,175;,
   3;42,161,176;,
   3;55,42,176;,
   3;57,178,177;,
   3;56,57,177;,
   3;58,179,178;,
   3;57,58,178;,
   3;59,180,179;,
   3;58,59,179;,
   3;60,181,180;,
   3;59,60,180;,
   3;61,182,181;,
   3;60,61,181;,
   3;62,183,182;,
   3;61,62,182;,
   3;63,184,183;,
   3;62,63,183;,
   3;64,185,184;,
   3;63,64,184;,
   3;65,186,185;,
   3;64,65,185;,
   3;66,187,186;,
   3;65,66,186;,
   3;67,188,187;,
   3;66,67,187;,
   3;68,189,188;,
   3;67,68,188;,
   3;69,190,189;,
   3;68,69,189;,
   3;56,177,190;,
   3;69,56,190;,
   3;70,71,57;,
   3;56,70,57;,
   3;71,73,58;,
   3;57,71,58;,
   3;73,74,59;,
   3;58,73,59;,
   3;74,75,60;,
   3;59,74,60;,
   3;75,76,61;,
   3;60,75,61;,
   3;76,77,62;,
   3;61,76,62;,
   3;77,78,63;,
   3;62,77,63;,
   3;78,79,64;,
   3;63,78,64;,
   3;79,80,65;,
   3;64,79,65;,
   3;80,81,66;,
   3;65,80,66;,
   3;81,82,67;,
   3;66,81,67;,
   3;82,83,68;,
   3;67,82,68;,
   3;83,84,69;,
   3;68,83,69;,
   3;84,70,56;,
   3;69,84,56;,
   3;96,91,94;,
   3;99,96,94;,
   3;98,88,86;,
   3;97,98,86;,
   3;90,85,87;,
   3;92,90,87;,
   3;93,89,90;,
   3;92,93,90;,
   3;94,91,191;,
   3;192,94,191;,
   3;100,95,96;,
   3;99,100,96;,
   3;98,97,193;,
   3;194,98,193;,
   3;197,196,195;,
   3;198,197,195;,
   3;201,200,199;,
   3;202,201,199;,
   3;205,204,203;,
   3;206,205,203;,
   3;207,206,203;,
   3;208,207,203;,
   3;211,210,209;,
   3;212,211,209;,
   3;200,214,213;,
   3;199,200,213;,
   3;104,110,109;,
   3;103,104,109;,
   3;106,216,215;,
   3;105,106,215;,
   3;111,218,217;,
   3;112,111,217;,
   3;219,111,110;,
   3;220,219,110;,
   3;222,109,221;,
   3;223,222,221;,
   3;110,114,113;,
   3;109,110,113;,
   3;111,115,114;,
   3;110,111,114;,
   3;115,111,112;,
   3;116,115,112;,
   3;109,113,224;,
   3;221,109,224;,
   3;114,102,101;,
   3;113,114,101;,
   3;115,108,225;,
   3;114,115,225;,
   3;226,115,116;,
   3;107,226,116;,
   3;113,228,227;,
   3;224,113,227;;
   MeshNormals
   {
    229;
    0.900969;0.433884;-0.000000;,
    0.623489;0.781832;-0.000000;,
    0.222520;0.974928;-0.000000;,
    -0.222520;0.974928;-0.000000;,
    -0.623489;0.781833;-0.000000;,
    -0.900969;0.433884;-0.000000;,
    -1.000000;-0.000000;0.000000;,
    -0.900969;-0.433884;0.000000;,
    -0.623491;-0.781831;0.000000;,
    -0.222522;-0.974928;-0.000000;,
    0.222522;-0.974928;-0.000000;,
    0.623490;-0.781831;0.000000;,
    0.900969;-0.433884;0.000000;,
    1.000000;-0.000001;0.000000;,
    0.801685;0.386073;-0.456343;,
    0.554783;0.695677;-0.456343;,
    0.197999;0.867495;-0.456343;,
    -0.197999;0.867495;-0.456343;,
    -0.554782;0.695677;-0.456343;,
    -0.801684;0.386073;-0.456344;,
    -0.889804;-0.000002;-0.456343;,
    -0.801686;-0.386070;-0.456343;,
    -0.554783;-0.695677;-0.456343;,
    -0.198001;-0.867494;-0.456343;,
    0.197999;-0.867495;-0.456344;,
    0.554786;-0.695675;-0.456343;,
    0.801685;-0.386073;-0.456343;,
    0.889804;-0.000001;-0.456343;,
    -0.801687;-0.386069;-0.456343;,
    0.000000;0.000000;-1.000000;,
    0.000000;0.000000;-1.000000;,
    0.000000;0.000000;-1.000000;,
    0.000000;0.000000;-1.000000;,
    0.000003;0.000003;-1.000000;,
    -0.000001;-0.000004;-1.000000;,
    -0.000001;0.000004;-1.000000;,
    0.000002;-0.000003;-1.000000;,
    0.000000;0.000000;-1.000000;,
    -0.000004;0.000000;-1.000000;,
    0.000004;0.000002;-1.000000;,
    -0.000003;-0.000003;-1.000000;,
    0.000000;0.000000;-1.000000;,
    -0.357811;-0.172311;-0.917758;,
    -0.247611;-0.310495;-0.917759;,
    -0.088370;-0.387180;-0.917759;,
    0.088370;-0.387180;-0.917759;,
    0.247611;-0.310495;-0.917759;,
    0.357811;-0.172313;-0.917758;,
    0.397140;0.000001;-0.917758;,
    0.357812;0.172313;-0.917758;,
    0.247619;0.310494;-0.917757;,
    0.088376;0.387186;-0.917756;,
    -0.088378;0.387187;-0.917756;,
    -0.247618;0.310496;-0.917757;,
    -0.357813;0.172312;-0.917757;,
    -0.397141;0.000001;-0.917758;,
    0.478086;0.230231;-0.847601;,
    0.330845;0.414867;-0.847601;,
    0.118075;0.517334;-0.847599;,
    -0.118076;0.517333;-0.847600;,
    -0.330843;0.414870;-0.847600;,
    -0.478084;0.230232;-0.847602;,
    -0.530632;-0.000001;-0.847602;,
    -0.478082;-0.230232;-0.847603;,
    -0.330842;-0.414864;-0.847603;,
    -0.118077;-0.517324;-0.847605;,
    0.118076;-0.517322;-0.847606;,
    0.330842;-0.414864;-0.847604;,
    0.478081;-0.230234;-0.847603;,
    0.530632;0.000000;-0.847603;,
    0.273373;0.131646;-0.952857;,
    0.189178;0.237222;-0.952858;,
    -0.000001;-0.000002;-1.000000;,
    0.067514;0.295813;-0.952857;,
    -0.067519;0.295811;-0.952857;,
    -0.189177;0.237226;-0.952857;,
    -0.273370;0.131648;-0.952858;,
    -0.303422;-0.000001;-0.952856;,
    -0.273372;-0.131646;-0.952857;,
    -0.189178;-0.237225;-0.952857;,
    -0.067518;-0.295813;-0.952857;,
    0.067518;-0.295813;-0.952857;,
    0.189178;-0.237225;-0.952857;,
    0.273370;-0.131651;-0.952857;,
    0.303420;-0.000000;-0.952857;,
    -1.000000;0.000000;-0.000000;,
    1.000000;0.000000;-0.000000;,
    -1.000000;0.000000;-0.000000;,
    1.000000;0.000000;-0.000000;,
    -0.937417;0.348208;-0.000000;,
    -0.877492;0.157710;-0.452919;,
    -0.191083;0.869386;-0.455692;,
    -0.877492;0.157710;0.452919;,
    -0.937417;0.348208;-0.000000;,
    -0.191083;0.869386;0.455692;,
    0.419324;0.907837;-0.000000;,
    0.191083;0.869386;-0.455692;,
    0.877492;0.157710;-0.452919;,
    0.877492;0.157710;0.452919;,
    0.191083;0.869386;0.455692;,
    0.419324;0.907837;-0.000000;,
    0.000000;0.000000;-1.000000;,
    0.000000;0.000000;-1.000000;,
    0.000000;0.000000;-1.000000;,
    0.000000;0.000000;-1.000000;,
    0.000000;1.000000;-0.000000;,
    0.000000;1.000000;-0.000000;,
    0.000000;0.000000;1.000000;,
    1.000000;0.000000;-0.000000;,
    -0.408248;-0.816497;-0.408248;,
    0.408248;-0.816497;-0.408248;,
    0.408248;-0.816497;0.408248;,
    -0.408248;-0.816497;0.408248;,
    -0.408248;-0.816497;-0.408248;,
    0.408248;-0.816497;-0.408248;,
    0.408248;-0.816497;0.408248;,
    -0.408248;-0.816497;0.408248;,
    -0.222520;0.974928;-0.000000;,
    -0.197999;0.867495;-0.456343;,
    0.000000;0.000000;-1.000000;,
    0.554783;0.695677;-0.456343;,
    0.000000;0.000000;-1.000000;,
    0.197999;0.867495;-0.456343;,
    -0.554783;-0.695677;-0.456344;,
    0.000000;0.000000;-1.000000;,
    -0.197999;0.867495;-0.456343;,
    -0.197996;-0.867496;-0.456343;,
    0.000000;0.000000;-1.000000;,
    -0.554782;0.695677;-0.456343;,
    0.197996;-0.867496;-0.456343;,
    0.000003;0.000003;-1.000000;,
    -0.801684;0.386073;-0.456344;,
    0.554782;-0.695678;-0.456344;,
    -0.000001;-0.000004;-1.000000;,
    -0.889804;-0.000002;-0.456343;,
    0.801685;-0.386072;-0.456343;,
    -0.000001;0.000004;-1.000000;,
    -0.801686;-0.386070;-0.456343;,
    0.889804;0.000003;-0.456344;,
    0.000002;-0.000003;-1.000000;,
    -0.554783;-0.695677;-0.456343;,
    0.801686;0.386070;-0.456343;,
    0.000000;0.000000;-1.000000;,
    -0.198001;-0.867494;-0.456343;,
    0.554787;0.695674;-0.456343;,
    -0.000004;0.000000;-1.000000;,
    0.197999;-0.867495;-0.456344;,
    0.198002;0.867494;-0.456344;,
    0.000004;0.000002;-1.000000;,
    0.554786;-0.695675;-0.456343;,
    -0.198002;0.867494;-0.456344;,
    -0.000003;-0.000003;-1.000000;,
    0.801685;-0.386073;-0.456343;,
    -0.554788;0.695673;-0.456343;,
    0.000000;0.000000;-1.000000;,
    0.889804;-0.000001;-0.456343;,
    -0.801685;0.386072;-0.456343;,
    -0.000001;0.000004;-1.000000;,
    0.801685;0.386073;-0.456343;,
    -0.000001;0.000004;-1.000000;,
    -0.889804;0.000002;-0.456343;,
    -0.801687;-0.386069;-0.456343;,
    -0.554783;-0.695677;-0.456344;,
    -0.197996;-0.867496;-0.456343;,
    0.197996;-0.867496;-0.456343;,
    0.197996;-0.867496;-0.456343;,
    0.554782;-0.695678;-0.456344;,
    0.088370;-0.387180;-0.917759;,
    0.801685;-0.386072;-0.456343;,
    0.889804;0.000003;-0.456344;,
    0.801686;0.386070;-0.456343;,
    0.554787;0.695674;-0.456343;,
    0.198002;0.867494;-0.456344;,
    -0.198002;0.867494;-0.456344;,
    -0.554788;0.695673;-0.456343;,
    -0.801685;0.386072;-0.456343;,
    -0.889804;0.000002;-0.456343;,
    -0.357811;-0.172311;-0.917758;,
    -0.247611;-0.310495;-0.917759;,
    -0.088370;-0.387180;-0.917759;,
    0.088370;-0.387180;-0.917759;,
    0.247611;-0.310495;-0.917759;,
    0.357811;-0.172313;-0.917758;,
    0.397140;0.000001;-0.917758;,
    0.357812;0.172313;-0.917758;,
    0.247619;0.310494;-0.917757;,
    0.088376;0.387186;-0.917756;,
    -0.088378;0.387187;-0.917756;,
    -0.247618;0.310496;-0.917757;,
    -0.357813;0.172312;-0.917757;,
    -0.397141;0.000001;-0.917758;,
    -0.419324;0.907837;-0.000000;,
    -0.419324;0.907837;-0.000000;,
    0.937417;0.348208;-0.000000;,
    0.937417;0.348208;-0.000000;,
    0.877492;0.157710;-0.452919;,
    0.000000;0.000000;-1.000000;,
    0.000000;0.000000;-1.000000;,
    0.000000;0.000000;-1.000000;,
    0.000000;0.000000;1.000000;,
    0.000000;0.000000;1.000000;,
    -0.191083;0.869386;0.455692;,
    0.191083;0.869386;0.455692;,
    0.000000;0.000000;-1.000000;,
    0.191083;0.869386;-0.455692;,
    -0.191083;0.869386;-0.455692;,
    0.000000;0.000000;-1.000000;,
    -0.877492;0.157710;-0.452919;,
    0.000000;0.000000;-1.000000;,
    0.000000;0.000000;1.000000;,
    0.000000;0.000000;1.000000;,
    0.000000;0.000000;1.000000;,
    -0.877492;0.157710;0.452919;,
    0.877492;0.157710;0.452919;,
    0.000000;0.000000;1.000000;,
    0.000000;1.000000;-0.000000;,
    0.000000;1.000000;-0.000000;,
    0.000000;0.000000;1.000000;,
    0.000000;0.000000;1.000000;,
    1.000000;0.000000;-0.000000;,
    1.000000;0.000000;-0.000000;,
    -0.408248;-0.816497;0.408248;,
    -1.000000;0.000000;-0.000000;,
    -1.000000;0.000000;-0.000000;,
    -0.408248;-0.816497;0.408248;,
    1.000000;0.000000;-0.000000;,
    0.000000;0.000000;1.000000;,
    -1.000000;0.000000;-0.000000;,
    -1.000000;0.000000;-0.000000;;
    206;
    3;15,1,0;,
    3;14,15,0;,
    3;16,2,1;,
    3;15,16,1;,
    3;17,3,2;,
    3;16,17,2;,
    3;18,4,117;,
    3;118,18,117;,
    3;19,5,4;,
    3;18,19,4;,
    3;20,6,5;,
    3;19,20,5;,
    3;21,7,6;,
    3;20,21,6;,
    3;22,8,7;,
    3;21,22,7;,
    3;23,9,8;,
    3;22,23,8;,
    3;24,10,9;,
    3;23,24,9;,
    3;25,11,10;,
    3;24,25,10;,
    3;26,12,11;,
    3;25,26,11;,
    3;27,13,12;,
    3;26,27,12;,
    3;14,0,13;,
    3;27,14,13;,
    3;72,71,70;,
    3;72,73,71;,
    3;72,74,73;,
    3;72,75,74;,
    3;72,76,75;,
    3;72,77,76;,
    3;72,78,77;,
    3;72,79,78;,
    3;72,80,79;,
    3;72,81,80;,
    3;72,82,81;,
    3;72,83,82;,
    3;72,84,83;,
    3;72,70,84;,
    3;29,120,119;,
    3;28,29,119;,
    3;30,122,121;,
    3;123,30,121;,
    3;31,125,124;,
    3;126,31,124;,
    3;32,128,127;,
    3;129,32,127;,
    3;33,131,130;,
    3;132,33,130;,
    3;34,134,133;,
    3;135,34,133;,
    3;35,137,136;,
    3;138,35,136;,
    3;36,140,139;,
    3;141,36,139;,
    3;37,143,142;,
    3;144,37,142;,
    3;38,146,145;,
    3;147,38,145;,
    3;39,149,148;,
    3;150,39,148;,
    3;40,152,151;,
    3;153,40,151;,
    3;41,155,154;,
    3;156,41,154;,
    3;159,158,157;,
    3;160,159,157;,
    3;43,162,161;,
    3;42,43,161;,
    3;44,163,162;,
    3;43,44,162;,
    3;45,164,163;,
    3;44,45,163;,
    3;46,166,165;,
    3;167,46,165;,
    3;47,168,166;,
    3;46,47,166;,
    3;48,169,168;,
    3;47,48,168;,
    3;49,170,169;,
    3;48,49,169;,
    3;50,171,170;,
    3;49,50,170;,
    3;51,172,171;,
    3;50,51,171;,
    3;52,173,172;,
    3;51,52,172;,
    3;53,174,173;,
    3;52,53,173;,
    3;54,175,174;,
    3;53,54,174;,
    3;55,176,175;,
    3;54,55,175;,
    3;42,161,176;,
    3;55,42,176;,
    3;57,178,177;,
    3;56,57,177;,
    3;58,179,178;,
    3;57,58,178;,
    3;59,180,179;,
    3;58,59,179;,
    3;60,181,180;,
    3;59,60,180;,
    3;61,182,181;,
    3;60,61,181;,
    3;62,183,182;,
    3;61,62,182;,
    3;63,184,183;,
    3;62,63,183;,
    3;64,185,184;,
    3;63,64,184;,
    3;65,186,185;,
    3;64,65,185;,
    3;66,187,186;,
    3;65,66,186;,
    3;67,188,187;,
    3;66,67,187;,
    3;68,189,188;,
    3;67,68,188;,
    3;69,190,189;,
    3;68,69,189;,
    3;56,177,190;,
    3;69,56,190;,
    3;70,71,57;,
    3;56,70,57;,
    3;71,73,58;,
    3;57,71,58;,
    3;73,74,59;,
    3;58,73,59;,
    3;74,75,60;,
    3;59,74,60;,
    3;75,76,61;,
    3;60,75,61;,
    3;76,77,62;,
    3;61,76,62;,
    3;77,78,63;,
    3;62,77,63;,
    3;78,79,64;,
    3;63,78,64;,
    3;79,80,65;,
    3;64,79,65;,
    3;80,81,66;,
    3;65,80,66;,
    3;81,82,67;,
    3;66,81,67;,
    3;82,83,68;,
    3;67,82,68;,
    3;83,84,69;,
    3;68,83,69;,
    3;84,70,56;,
    3;69,84,56;,
    3;96,91,94;,
    3;99,96,94;,
    3;98,88,86;,
    3;97,98,86;,
    3;90,85,87;,
    3;92,90,87;,
    3;93,89,90;,
    3;92,93,90;,
    3;94,91,191;,
    3;192,94,191;,
    3;100,95,96;,
    3;99,100,96;,
    3;98,97,193;,
    3;194,98,193;,
    3;197,196,195;,
    3;198,197,195;,
    3;201,200,199;,
    3;202,201,199;,
    3;205,204,203;,
    3;206,205,203;,
    3;207,206,203;,
    3;208,207,203;,
    3;211,210,209;,
    3;212,211,209;,
    3;200,214,213;,
    3;199,200,213;,
    3;104,110,109;,
    3;103,104,109;,
    3;106,216,215;,
    3;105,106,215;,
    3;111,218,217;,
    3;112,111,217;,
    3;219,111,110;,
    3;220,219,110;,
    3;222,109,221;,
    3;223,222,221;,
    3;110,114,113;,
    3;109,110,113;,
    3;111,115,114;,
    3;110,111,114;,
    3;115,111,112;,
    3;116,115,112;,
    3;109,113,224;,
    3;221,109,224;,
    3;114,102,101;,
    3;113,114,101;,
    3;115,108,225;,
    3;114,115,225;,
    3;226,115,116;,
    3;107,226,116;,
    3;113,228,227;,
    3;224,113,227;;
   }
   MeshTextureCoords
   {
    229;
    0.280411;0.112633;,
    0.295503;0.112633;,
    0.310595;0.112633;,
    0.325687;0.112633;,
    0.129490;0.112633;,
    0.144582;0.112633;,
    0.159674;0.112633;,
    0.174766;0.112633;,
    0.189858;0.112633;,
    0.204950;0.112633;,
    0.220042;0.112633;,
    0.235135;0.112633;,
    0.250227;0.112633;,
    0.265319;0.112633;,
    0.280411;0.006988;,
    0.295503;0.006988;,
    0.310595;0.006988;,
    0.325687;0.006988;,
    0.129490;0.006988;,
    0.144582;0.006989;,
    0.159674;0.006988;,
    0.174766;0.006989;,
    0.189858;0.006988;,
    0.204950;0.006988;,
    0.220042;0.006989;,
    0.235135;0.006988;,
    0.250227;0.006989;,
    0.265319;0.006989;,
    0.091077;0.048470;,
    0.082103;0.037217;,
    0.069136;0.030973;,
    0.054744;0.030973;,
    0.041777;0.037217;,
    0.032804;0.048470;,
    0.029601;0.062501;,
    0.032804;0.076533;,
    0.041777;0.087785;,
    0.054744;0.094029;,
    0.069136;0.094029;,
    0.082103;0.087785;,
    0.091077;0.076533;,
    0.094279;0.062501;,
    0.083835;0.014135;,
    0.090104;0.014135;,
    0.096374;0.014135;,
    0.102643;0.014135;,
    0.021140;0.014135;,
    0.027410;0.014135;,
    0.033679;0.014135;,
    0.039949;0.014135;,
    0.046218;0.014135;,
    0.052488;0.014135;,
    0.058757;0.014135;,
    0.065027;0.014135;,
    0.071296;0.014135;,
    0.077565;0.014135;,
    0.084950;0.051421;,
    0.077863;0.042534;,
    0.067623;0.037603;,
    0.056257;0.037603;,
    0.046018;0.042534;,
    0.038931;0.051420;,
    0.036402;0.062501;,
    0.038931;0.073582;,
    0.046018;0.082467;,
    0.056258;0.087399;,
    0.067623;0.087399;,
    0.077863;0.082467;,
    0.084950;0.073582;,
    0.087479;0.062501;,
    0.072385;0.057471;,
    0.069169;0.053437;,
    0.061941;0.062501;,
    0.064520;0.051198;,
    0.059360;0.051198;,
    0.054712;0.053437;,
    0.051495;0.057471;,
    0.050347;0.062501;,
    0.051495;0.067531;,
    0.054712;0.071565;,
    0.059361;0.073804;,
    0.064520;0.073804;,
    0.069169;0.071565;,
    0.072385;0.067531;,
    0.073534;0.062501;,
    0.236484;0.220138;,
    0.236484;0.220138;,
    0.328192;0.220138;,
    0.328192;0.220138;,
    0.236484;0.129260;,
    0.236484;0.149409;,
    0.363897;0.231852;,
    0.328192;0.149409;,
    0.328192;0.129260;,
    0.363897;0.118938;,
    0.445503;0.231852;,
    0.424190;0.231852;,
    0.236484;0.149409;,
    0.328192;0.149409;,
    0.424190;0.118938;,
    0.445503;0.118938;,
    0.400094;0.114703;,
    0.449029;0.114703;,
    0.400094;0.003139;,
    0.449029;0.003139;,
    0.546897;0.003139;,
    0.497963;0.003139;,
    0.546897;0.114703;,
    0.497963;0.114703;,
    0.400094;0.029040;,
    0.449029;0.029040;,
    0.497963;0.029040;,
    0.546897;0.029040;,
    0.400094;0.029040;,
    0.449029;0.029040;,
    0.497963;0.029040;,
    0.546897;0.029040;,
    0.114398;0.112633;,
    0.114398;0.006988;,
    0.101344;0.043525;,
    0.089209;0.028308;,
    0.089209;0.028308;,
    0.071672;0.019863;,
    0.082103;0.037217;,
    0.071672;0.019863;,
    0.052209;0.019863;,
    0.069136;0.030973;,
    0.052209;0.019863;,
    0.034672;0.028308;,
    0.054744;0.030973;,
    0.034672;0.028308;,
    0.022537;0.043525;,
    0.041777;0.037217;,
    0.022537;0.043525;,
    0.018205;0.062501;,
    0.032804;0.048470;,
    0.018205;0.062501;,
    0.022537;0.081477;,
    0.029601;0.062501;,
    0.022537;0.081477;,
    0.034672;0.096695;,
    0.032804;0.076533;,
    0.034672;0.096695;,
    0.052209;0.105140;,
    0.041777;0.087785;,
    0.052209;0.105140;,
    0.071672;0.105140;,
    0.054744;0.094029;,
    0.071672;0.105140;,
    0.089209;0.096695;,
    0.069136;0.094029;,
    0.089209;0.096695;,
    0.101344;0.081477;,
    0.082103;0.087785;,
    0.101344;0.081477;,
    0.105675;0.062501;,
    0.091077;0.076533;,
    0.105675;0.062501;,
    0.101344;0.043525;,
    0.091077;0.048470;,
    0.094279;0.062501;,
    0.083835;0.006969;,
    0.090104;0.006969;,
    0.096374;0.006969;,
    0.102643;0.006969;,
    0.014871;0.006969;,
    0.021140;0.006969;,
    0.014871;0.014135;,
    0.027410;0.006969;,
    0.033679;0.006969;,
    0.039949;0.006969;,
    0.046218;0.006969;,
    0.052488;0.006969;,
    0.058757;0.006969;,
    0.065027;0.006969;,
    0.071296;0.006969;,
    0.077565;0.006969;,
    0.091077;0.048470;,
    0.082103;0.037217;,
    0.069136;0.030973;,
    0.054744;0.030973;,
    0.041777;0.037217;,
    0.032804;0.048470;,
    0.029601;0.062501;,
    0.032804;0.076533;,
    0.041777;0.087785;,
    0.054744;0.094029;,
    0.069136;0.094029;,
    0.082104;0.087785;,
    0.091077;0.076533;,
    0.094279;0.062501;,
    0.342585;0.231852;,
    0.342585;0.118938;,
    0.236484;0.129260;,
    0.328192;0.129260;,
    0.221774;0.151574;,
    0.127110;0.151574;,
    0.127110;0.220138;,
    0.221774;0.220138;,
    0.107514;0.133930;,
    0.027207;0.133930;,
    0.043837;0.126248;,
    0.090884;0.126248;,
    0.214843;0.132916;,
    0.198111;0.125187;,
    0.150774;0.125187;,
    0.134041;0.132916;,
    0.127110;0.151574;,
    0.221774;0.151574;,
    0.114403;0.152474;,
    0.114403;0.220138;,
    0.020318;0.220138;,
    0.020318;0.152474;,
    0.114403;0.152474;,
    0.020318;0.152474;,
    0.595832;0.003139;,
    0.561667;0.003139;,
    0.546897;0.003139;,
    0.497963;0.003139;,
    0.497963;0.003139;,
    0.449029;0.003139;,
    0.351160;0.029040;,
    0.400094;0.003139;,
    0.351160;0.003139;,
    0.351160;0.029040;,
    0.449029;0.114703;,
    0.497963;0.114703;,
    0.351160;0.114703;,
    0.400094;0.114703;;
   }
   MeshMaterialList
   {
    1;
    206;
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
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
    229;
    117;
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
    98,
    99,
    100,
    101,
    102,
    103,
    104,
    105,
    106,
    107,
    108,
    109,
    110,
    111,
    112,
    113,
    114,
    115,
    116,
    3,
    17,
    14,
    15,
    15,
    16,
    29,
    16,
    17,
    30,
    17,
    18,
    31,
    18,
    19,
    32,
    19,
    20,
    33,
    20,
    21,
    34,
    21,
    22,
    35,
    22,
    23,
    36,
    23,
    24,
    37,
    24,
    25,
    38,
    25,
    26,
    39,
    26,
    27,
    40,
    27,
    14,
    28,
    41,
    28,
    29,
    30,
    31,
    31,
    32,
    45,
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
    89,
    93,
    95,
    100,
    97,
    90,
    85,
    86,
    100,
    93,
    94,
    99,
    95,
    96,
    91,
    89,
    90,
    97,
    98,
    88,
    87,
    92,
    98,
    92,
    103,
    104,
    105,
    106,
    106,
    104,
    112,
    103,
    105,
    116,
    102,
    108,
    107,
    101;
   }
  }
 }
}

# Animations definition.

