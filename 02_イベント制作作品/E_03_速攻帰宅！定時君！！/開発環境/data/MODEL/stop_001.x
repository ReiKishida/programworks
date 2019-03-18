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
  "data/TEXTURE/Stop_col.png";
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
   0.011674,0.000000,0.000000,0.000000,
   0.000000,0.011674,0.000000,0.000000,
   0.000000,0.000000,0.011674,0.000000,
   0.000000,0.000000,0.000000,1.000000;;
  }
  Mesh
  {
   96;
   1.366259;0.000000;-0.241797;,
   0.844397;0.000000;1.364328;,
   -0.844383;0.000000;1.364328;,
   -1.366244;0.000000;-0.241797;,
   0.000007;0.000000;-1.234437;,
   1.366259;85.747604;-0.241797;,
   0.844397;85.747604;1.364328;,
   -0.844383;85.747604;1.364328;,
   -1.366244;85.747604;-0.241797;,
   0.000007;85.747604;-1.234437;,
   0.000007;0.000000;0.202125;,
   0.000007;85.747604;0.202125;,
   -14.234641;70.650314;-1.364328;,
   -14.234641;70.650314;-1.147901;,
   -5.965520;62.283127;-1.364328;,
   -5.965520;62.283127;-1.147901;,
   5.798107;62.213779;-1.364328;,
   5.798107;62.213779;-1.147916;,
   14.165289;70.482903;-1.364328;,
   14.165289;70.482903;-1.147916;,
   14.234641;82.246529;-1.364328;,
   14.234641;82.246529;-1.147916;,
   5.965535;90.613716;-1.364328;,
   5.965535;90.613716;-1.147916;,
   -5.798092;90.683067;-1.364328;,
   -5.798092;90.683067;-1.147916;,
   -14.165276;82.413956;-1.364328;,
   -14.165276;82.413956;-1.147916;,
   0.000002;76.448433;-1.147910;,
   0.844397;0.000000;1.364328;,
   0.844397;85.747604;1.364328;,
   -0.844383;0.000000;1.364328;,
   -0.844383;85.747604;1.364328;,
   -1.366244;0.000000;-0.241797;,
   -1.366244;85.747604;-0.241797;,
   0.000007;0.000000;-1.234437;,
   1.366259;0.000000;-0.241797;,
   1.366259;85.747604;-0.241797;,
   0.000007;85.747604;-1.234437;,
   0.844397;0.000000;1.364328;,
   1.366259;0.000000;-0.241797;,
   -0.844383;0.000000;1.364328;,
   -1.366244;0.000000;-0.241797;,
   0.000007;0.000000;-1.234437;,
   1.366259;85.747604;-0.241797;,
   0.844397;85.747604;1.364328;,
   -0.844383;85.747604;1.364328;,
   -1.366244;85.747604;-0.241797;,
   0.000007;85.747604;-1.234437;,
   -5.965520;62.283127;-1.364328;,
   -5.965520;62.283127;-1.147901;,
   5.798107;62.213779;-1.364328;,
   5.798107;62.213779;-1.147916;,
   14.165289;70.482903;-1.364328;,
   14.165289;70.482903;-1.147916;,
   14.234641;82.246529;-1.364328;,
   14.234641;82.246529;-1.147916;,
   5.965535;90.613716;-1.364328;,
   5.965535;90.613716;-1.147916;,
   -14.234641;70.650314;-1.147901;,
   -14.234641;70.650314;-1.364328;,
   -5.798092;90.683067;-1.364328;,
   -5.798092;90.683067;-1.147916;,
   -14.165276;82.413956;-1.147916;,
   -14.165276;82.413956;-1.364328;,
   -14.234641;70.650314;-1.364328;,
   -5.965520;62.283127;-1.364328;,
   5.798107;62.213779;-1.364328;,
   14.165289;70.482903;-1.364328;,
   14.234641;82.246529;-1.364328;,
   5.965535;90.613716;-1.364328;,
   -5.798092;90.683067;-1.364328;,
   -14.165276;82.413956;-1.364328;,
   -5.965520;62.283127;-1.147901;,
   -14.234641;70.650314;-1.147901;,
   5.798107;62.213779;-1.147916;,
   -5.965520;62.283127;-1.147901;,
   0.000002;76.448433;-1.147910;,
   14.165289;70.482903;-1.147916;,
   5.798107;62.213779;-1.147916;,
   0.000002;76.448433;-1.147910;,
   14.234641;82.246529;-1.147916;,
   14.165289;70.482903;-1.147916;,
   0.000002;76.448433;-1.147910;,
   5.965535;90.613716;-1.147916;,
   14.234641;82.246529;-1.147916;,
   0.000002;76.448433;-1.147910;,
   -5.798092;90.683067;-1.147916;,
   5.965535;90.613716;-1.147916;,
   0.000002;76.448433;-1.147910;,
   -14.234641;70.650314;-1.147901;,
   -14.165276;82.413956;-1.147916;,
   0.000002;76.448433;-1.147910;,
   -14.165276;82.413956;-1.147916;,
   -5.798092;90.683067;-1.147916;,
   0.000002;76.448433;-1.147910;;
   50;
   3;6,1,0;,
   3;5,6,0;,
   3;7,2,29;,
   3;30,7,29;,
   3;8,3,31;,
   3;32,8,31;,
   3;9,4,33;,
   3;34,9,33;,
   3;37,36,35;,
   3;38,37,35;,
   3;10,40,39;,
   3;10,39,41;,
   3;10,41,42;,
   3;10,42,43;,
   3;10,43,40;,
   3;11,45,44;,
   3;11,46,45;,
   3;11,47,46;,
   3;11,48,47;,
   3;11,44,48;,
   3;15,13,12;,
   3;14,15,12;,
   3;17,50,49;,
   3;16,17,49;,
   3;19,52,51;,
   3;18,19,51;,
   3;21,54,53;,
   3;20,21,53;,
   3;23,56,55;,
   3;22,23,55;,
   3;25,58,57;,
   3;24,25,57;,
   3;59,27,26;,
   3;60,59,26;,
   3;63,62,61;,
   3;64,63,61;,
   3;67,66,65;,
   3;68,67,65;,
   3;69,68,65;,
   3;70,69,65;,
   3;71,70,65;,
   3;72,71,65;,
   3;28,74,73;,
   3;77,76,75;,
   3;80,79,78;,
   3;83,82,81;,
   3;86,85,84;,
   3;89,88,87;,
   3;92,91,90;,
   3;95,94,93;;
   MeshNormals
   {
    96;
    0.951057;0.000000;0.309017;,
    0.951057;0.000000;0.309017;,
    0.000000;0.000000;1.000000;,
    -0.951057;0.000000;0.309017;,
    -0.587785;0.000000;-0.809017;,
    0.951057;0.000000;0.309017;,
    0.951057;0.000000;0.309017;,
    0.000000;0.000000;1.000000;,
    -0.951057;0.000000;0.309017;,
    -0.587785;0.000000;-0.809017;,
    0.000000;-1.000000;-0.000000;,
    0.000000;1.000000;-0.000000;,
    -0.711263;-0.702926;-0.000000;,
    -0.711263;-0.702926;-0.000000;,
    -0.711263;-0.702926;-0.000000;,
    -0.711263;-0.702926;-0.000000;,
    -0.005895;-0.999983;-0.000000;,
    -0.005895;-0.999983;-0.000000;,
    0.702927;-0.711262;-0.000000;,
    0.702927;-0.711262;-0.000000;,
    0.999983;-0.005895;-0.000000;,
    0.999983;-0.005895;-0.000000;,
    0.711263;0.702926;-0.000000;,
    0.711263;0.702926;-0.000000;,
    0.005895;0.999983;-0.000000;,
    0.005895;0.999983;-0.000000;,
    -0.999983;0.005896;-0.000000;,
    -0.999983;0.005896;-0.000000;,
    0.000000;0.000000;1.000000;,
    0.000000;0.000000;1.000000;,
    0.000000;0.000000;1.000000;,
    -0.951057;0.000000;0.309017;,
    -0.951057;0.000000;0.309017;,
    -0.587785;0.000000;-0.809017;,
    -0.587785;0.000000;-0.809017;,
    0.587785;0.000000;-0.809017;,
    0.587785;0.000000;-0.809017;,
    0.587785;0.000000;-0.809017;,
    0.587785;0.000000;-0.809017;,
    0.000000;-1.000000;-0.000000;,
    0.000000;-1.000000;-0.000000;,
    0.000000;-1.000000;-0.000000;,
    0.000000;-1.000000;-0.000000;,
    0.000000;-1.000000;-0.000000;,
    0.000000;1.000000;-0.000000;,
    0.000000;1.000000;-0.000000;,
    0.000000;1.000000;-0.000000;,
    0.000000;1.000000;-0.000000;,
    0.000000;1.000000;-0.000000;,
    -0.005895;-0.999983;-0.000000;,
    -0.005895;-0.999983;-0.000000;,
    0.702927;-0.711262;-0.000000;,
    0.702927;-0.711262;-0.000000;,
    0.999983;-0.005895;-0.000000;,
    0.999983;-0.005895;-0.000000;,
    0.711263;0.702926;-0.000000;,
    0.711263;0.702926;-0.000000;,
    0.005895;0.999983;-0.000000;,
    0.005895;0.999983;-0.000000;,
    -0.999983;0.005896;-0.000000;,
    -0.999983;0.005896;-0.000000;,
    -0.702926;0.711263;-0.000000;,
    -0.702926;0.711263;-0.000000;,
    -0.702926;0.711263;-0.000000;,
    -0.702926;0.711263;-0.000000;,
    0.000000;0.000000;-1.000000;,
    0.000000;0.000000;-1.000000;,
    0.000000;0.000000;-1.000000;,
    0.000000;0.000000;-1.000000;,
    0.000000;0.000000;-1.000000;,
    0.000000;0.000000;-1.000000;,
    0.000000;0.000000;-1.000000;,
    0.000000;0.000000;-1.000000;,
    0.000000;0.000000;1.000000;,
    0.000000;0.000000;1.000000;,
    0.000001;0.000000;1.000000;,
    0.000001;0.000000;1.000000;,
    0.000001;0.000000;1.000000;,
    0.000000;-0.000000;1.000000;,
    0.000000;-0.000000;1.000000;,
    0.000000;-0.000000;1.000000;,
    0.000000;-0.000000;1.000000;,
    0.000000;-0.000000;1.000000;,
    0.000000;-0.000000;1.000000;,
    0.000000;0.000000;1.000000;,
    0.000000;0.000000;1.000000;,
    0.000000;0.000000;1.000000;,
    0.000000;0.000000;1.000000;,
    0.000000;0.000000;1.000000;,
    0.000000;0.000000;1.000000;,
    0.000000;0.000001;1.000000;,
    0.000000;0.000001;1.000000;,
    0.000000;0.000001;1.000000;,
    -0.000000;0.000000;1.000000;,
    -0.000000;0.000000;1.000000;,
    -0.000000;0.000000;1.000000;;
    50;
    3;6,1,0;,
    3;5,6,0;,
    3;7,2,29;,
    3;30,7,29;,
    3;8,3,31;,
    3;32,8,31;,
    3;9,4,33;,
    3;34,9,33;,
    3;37,36,35;,
    3;38,37,35;,
    3;10,40,39;,
    3;10,39,41;,
    3;10,41,42;,
    3;10,42,43;,
    3;10,43,40;,
    3;11,45,44;,
    3;11,46,45;,
    3;11,47,46;,
    3;11,48,47;,
    3;11,44,48;,
    3;15,13,12;,
    3;14,15,12;,
    3;17,50,49;,
    3;16,17,49;,
    3;19,52,51;,
    3;18,19,51;,
    3;21,54,53;,
    3;20,21,53;,
    3;23,56,55;,
    3;22,23,55;,
    3;25,58,57;,
    3;24,25,57;,
    3;59,27,26;,
    3;60,59,26;,
    3;63,62,61;,
    3;64,63,61;,
    3;67,66,65;,
    3;68,67,65;,
    3;69,68,65;,
    3;70,69,65;,
    3;71,70,65;,
    3;72,71,65;,
    3;28,74,73;,
    3;77,76,75;,
    3;80,79,78;,
    3;83,82,81;,
    3;86,85,84;,
    3;89,88,87;,
    3;92,91,90;,
    3;95,94,93;;
   }
   MeshTextureCoords
   {
    96;
    0.917854;0.959060;,
    0.915830;0.959060;,
    0.909279;0.959060;,
    0.909751;0.943168;,
    0.910058;0.943168;,
    0.917854;0.925328;,
    0.915830;0.925328;,
    0.909279;0.925328;,
    0.909751;0.941220;,
    0.910058;0.941220;,
    0.912554;1.009696;,
    0.912554;0.874692;,
    0.978845;0.703087;,
    0.000000;0.709295;,
    0.700677;0.988569;,
    0.290458;1.003737;,
    0.304955;0.990935;,
    0.703662;1.006178;,
    0.023488;0.708799;,
    0.997564;0.715187;,
    0.021155;0.307432;,
    1.000000;0.301223;,
    0.299323;0.021950;,
    0.709543;0.006781;,
    0.695045;0.019584;,
    0.296339;0.004341;,
    0.976512;0.301719;,
    0.002436;0.295332;,
    0.500926;0.515944;,
    0.915830;0.959060;,
    0.915830;0.925328;,
    0.909279;0.959060;,
    0.909279;0.925328;,
    0.909751;0.943168;,
    0.909751;0.941220;,
    0.910058;0.943168;,
    0.917854;0.959060;,
    0.917854;0.925328;,
    0.910058;0.941220;,
    0.915830;0.959060;,
    0.917854;0.959060;,
    0.909279;0.959060;,
    0.909751;0.943168;,
    0.910058;0.943168;,
    0.917854;0.925328;,
    0.915830;0.925328;,
    0.909279;0.925328;,
    0.909751;0.941220;,
    0.910058;0.941220;,
    0.700677;0.988569;,
    0.290458;1.003737;,
    0.304955;0.990935;,
    0.703662;1.006178;,
    0.023488;0.708799;,
    0.997564;0.715187;,
    0.021155;0.307432;,
    1.000000;0.301223;,
    0.299323;0.021950;,
    0.709543;0.006781;,
    0.000000;0.709295;,
    0.978845;0.703087;,
    0.695045;0.019584;,
    0.296339;0.004341;,
    0.002436;0.295332;,
    0.976512;0.301719;,
    0.978845;0.703087;,
    0.700677;0.988569;,
    0.304955;0.990935;,
    0.023488;0.708799;,
    0.021155;0.307432;,
    0.299323;0.021950;,
    0.695045;0.019584;,
    0.976512;0.301719;,
    0.301765;0.995311;,
    0.024647;0.712605;,
    0.695549;0.997210;,
    0.301765;0.995311;,
    0.500926;0.515944;,
    0.975325;0.717190;,
    0.695549;0.997210;,
    0.500926;0.515944;,
    0.977205;0.319283;,
    0.975325;0.717190;,
    0.500926;0.515944;,
    0.700087;0.036577;,
    0.977205;0.319283;,
    0.500926;0.515944;,
    0.306303;0.034678;,
    0.700087;0.036577;,
    0.500926;0.515944;,
    0.024647;0.712605;,
    0.026527;0.314698;,
    0.500926;0.515944;,
    0.026527;0.314698;,
    0.306303;0.034678;,
    0.500926;0.515944;;
   }
   MeshMaterialList
   {
    1;
    50;
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
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
    96;
    29;
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
    1,
    6,
    2,
    7,
    3,
    8,
    4,
    0,
    5,
    9,
    1,
    0,
    2,
    3,
    4,
    5,
    6,
    7,
    8,
    9,
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
    13,
    12,
    24,
    25,
    27,
    26,
    12,
    14,
    16,
    18,
    20,
    22,
    24,
    26,
    15,
    13,
    17,
    15,
    28,
    19,
    17,
    28,
    21,
    19,
    28,
    23,
    21,
    28,
    25,
    23,
    28,
    13,
    27,
    28,
    27,
    25,
    28;
   }
  }
 }
}

# Animations definition.

