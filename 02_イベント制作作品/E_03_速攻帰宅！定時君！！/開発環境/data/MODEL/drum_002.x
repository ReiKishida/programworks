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
  "data/TEXTURE/drum1_base_color.jpg";
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
   32.929153,0.000000,0.000000,0.000000,
   0.000000,45.185940,0.000000,0.000000,
   0.000000,0.000000,33.435551,0.000000,
   0.000000,36.148750,0.000000,1.000000;;
  }
  Mesh
  {
   124;
   0.760846;-0.800000;0.247214;,
   0.647214;-0.800000;0.470228;,
   0.470228;-0.800000;0.647214;,
   0.247214;-0.800000;0.760846;,
   0.000000;-0.800000;0.800000;,
   -0.247214;-0.800000;0.760846;,
   -0.470228;-0.800000;0.647214;,
   -0.647214;-0.800000;0.470228;,
   -0.760845;-0.800000;0.247214;,
   -0.800000;-0.800000;-0.000000;,
   -0.760845;-0.800000;-0.247214;,
   -0.647214;-0.800000;-0.470228;,
   -0.470228;-0.800000;-0.647214;,
   -0.247214;-0.800000;-0.760845;,
   -0.000000;-0.800000;-0.800000;,
   0.247214;-0.800000;-0.760845;,
   0.470228;-0.800000;-0.647214;,
   0.647214;-0.800000;-0.470228;,
   0.760845;-0.800000;-0.247214;,
   0.800000;-0.800000;-0.000000;,
   0.760846;0.800000;0.247214;,
   0.647214;0.800000;0.470228;,
   0.470228;0.800000;0.647214;,
   0.247214;0.800000;0.760846;,
   0.000000;0.800000;0.800000;,
   -0.247214;0.800000;0.760846;,
   -0.470228;0.800000;0.647214;,
   -0.647214;0.800000;0.470228;,
   -0.760845;0.800000;0.247214;,
   -0.800000;0.800000;-0.000000;,
   -0.760845;0.800000;-0.247214;,
   -0.647214;0.800000;-0.470228;,
   -0.470228;0.800000;-0.647214;,
   -0.247214;0.800000;-0.760845;,
   -0.000000;0.800000;-0.800000;,
   0.247214;0.800000;-0.760845;,
   0.470228;0.800000;-0.647214;,
   0.647214;0.800000;-0.470228;,
   0.760845;0.800000;-0.247214;,
   0.800000;0.800000;-0.000000;,
   0.000000;-0.800000;-0.000000;,
   0.000000;0.800000;-0.000000;,
   0.760846;-0.800000;0.247214;,
   0.760846;0.800000;0.247214;,
   0.647214;-0.800000;0.470228;,
   0.760846;-0.800000;0.247214;,
   0.470228;-0.800000;0.647214;,
   0.647214;-0.800000;0.470228;,
   0.247214;-0.800000;0.760846;,
   0.470228;-0.800000;0.647214;,
   0.000000;-0.800000;0.800000;,
   0.247214;-0.800000;0.760846;,
   -0.247214;-0.800000;0.760846;,
   0.000000;-0.800000;0.800000;,
   -0.470228;-0.800000;0.647214;,
   -0.247214;-0.800000;0.760846;,
   -0.647214;-0.800000;0.470228;,
   -0.470228;-0.800000;0.647214;,
   -0.760845;-0.800000;0.247214;,
   -0.647214;-0.800000;0.470228;,
   -0.800000;-0.800000;-0.000000;,
   -0.760845;-0.800000;0.247214;,
   -0.760845;-0.800000;-0.247214;,
   -0.800000;-0.800000;-0.000000;,
   -0.647214;-0.800000;-0.470228;,
   -0.760845;-0.800000;-0.247214;,
   -0.470228;-0.800000;-0.647214;,
   -0.647214;-0.800000;-0.470228;,
   -0.247214;-0.800000;-0.760845;,
   -0.470228;-0.800000;-0.647214;,
   -0.000000;-0.800000;-0.800000;,
   -0.247214;-0.800000;-0.760845;,
   0.247214;-0.800000;-0.760845;,
   -0.000000;-0.800000;-0.800000;,
   0.470228;-0.800000;-0.647214;,
   0.247214;-0.800000;-0.760845;,
   0.647214;-0.800000;-0.470228;,
   0.470228;-0.800000;-0.647214;,
   0.760845;-0.800000;-0.247214;,
   0.647214;-0.800000;-0.470228;,
   0.800000;-0.800000;-0.000000;,
   0.760845;-0.800000;-0.247214;,
   0.760846;-0.800000;0.247214;,
   0.800000;-0.800000;-0.000000;,
   0.760846;0.800000;0.247214;,
   0.647214;0.800000;0.470228;,
   0.647214;0.800000;0.470228;,
   0.470228;0.800000;0.647214;,
   0.470228;0.800000;0.647214;,
   0.247214;0.800000;0.760846;,
   0.247214;0.800000;0.760846;,
   0.000000;0.800000;0.800000;,
   0.000000;0.800000;0.800000;,
   -0.247214;0.800000;0.760846;,
   -0.247214;0.800000;0.760846;,
   -0.470228;0.800000;0.647214;,
   -0.470228;0.800000;0.647214;,
   -0.647214;0.800000;0.470228;,
   -0.647214;0.800000;0.470228;,
   -0.760845;0.800000;0.247214;,
   -0.760845;0.800000;0.247214;,
   -0.800000;0.800000;-0.000000;,
   -0.800000;0.800000;-0.000000;,
   -0.760845;0.800000;-0.247214;,
   -0.760845;0.800000;-0.247214;,
   -0.647214;0.800000;-0.470228;,
   -0.647214;0.800000;-0.470228;,
   -0.470228;0.800000;-0.647214;,
   -0.470228;0.800000;-0.647214;,
   -0.247214;0.800000;-0.760845;,
   -0.247214;0.800000;-0.760845;,
   -0.000000;0.800000;-0.800000;,
   -0.000000;0.800000;-0.800000;,
   0.247214;0.800000;-0.760845;,
   0.247214;0.800000;-0.760845;,
   0.470228;0.800000;-0.647214;,
   0.470228;0.800000;-0.647214;,
   0.647214;0.800000;-0.470228;,
   0.647214;0.800000;-0.470228;,
   0.760845;0.800000;-0.247214;,
   0.760845;0.800000;-0.247214;,
   0.800000;0.800000;-0.000000;,
   0.800000;0.800000;-0.000000;,
   0.760846;0.800000;0.247214;;
   80;
   3;21,1,0;,
   3;20,21,0;,
   3;22,2,1;,
   3;21,22,1;,
   3;23,3,2;,
   3;22,23,2;,
   3;24,4,3;,
   3;23,24,3;,
   3;25,5,4;,
   3;24,25,4;,
   3;26,6,5;,
   3;25,26,5;,
   3;27,7,6;,
   3;26,27,6;,
   3;28,8,7;,
   3;27,28,7;,
   3;29,9,8;,
   3;28,29,8;,
   3;30,10,9;,
   3;29,30,9;,
   3;31,11,10;,
   3;30,31,10;,
   3;32,12,11;,
   3;31,32,11;,
   3;33,13,12;,
   3;32,33,12;,
   3;34,14,13;,
   3;33,34,13;,
   3;35,15,14;,
   3;34,35,14;,
   3;36,16,15;,
   3;35,36,15;,
   3;37,17,16;,
   3;36,37,16;,
   3;38,18,17;,
   3;37,38,17;,
   3;39,19,18;,
   3;38,39,18;,
   3;43,42,19;,
   3;39,43,19;,
   3;40,45,44;,
   3;40,47,46;,
   3;40,49,48;,
   3;40,51,50;,
   3;40,53,52;,
   3;40,55,54;,
   3;40,57,56;,
   3;40,59,58;,
   3;40,61,60;,
   3;40,63,62;,
   3;40,65,64;,
   3;40,67,66;,
   3;40,69,68;,
   3;40,71,70;,
   3;40,73,72;,
   3;40,75,74;,
   3;40,77,76;,
   3;40,79,78;,
   3;40,81,80;,
   3;40,83,82;,
   3;41,85,84;,
   3;41,87,86;,
   3;41,89,88;,
   3;41,91,90;,
   3;41,93,92;,
   3;41,95,94;,
   3;41,97,96;,
   3;41,99,98;,
   3;41,101,100;,
   3;41,103,102;,
   3;41,105,104;,
   3;41,107,106;,
   3;41,109,108;,
   3;41,111,110;,
   3;41,113,112;,
   3;41,115,114;,
   3;41,117,116;,
   3;41,119,118;,
   3;41,121,120;,
   3;41,123,122;;
   MeshNormals
   {
    124;
    0.848525;-0.451657;0.275701;,
    0.721798;-0.451657;0.524417;,
    0.524417;-0.451657;0.721799;,
    0.275702;-0.451657;0.848525;,
    -0.000000;-0.451657;0.892192;,
    -0.275703;-0.451657;0.848525;,
    -0.524417;-0.451657;0.721798;,
    -0.721799;-0.451657;0.524417;,
    -0.848525;-0.451657;0.275702;,
    -0.892192;-0.451657;-0.000000;,
    -0.848525;-0.451657;-0.275702;,
    -0.721798;-0.451657;-0.524417;,
    -0.524417;-0.451657;-0.721798;,
    -0.275702;-0.451657;-0.848525;,
    0.000000;-0.451657;-0.892192;,
    0.275703;-0.451657;-0.848525;,
    0.524417;-0.451657;-0.721798;,
    0.721798;-0.451657;-0.524417;,
    0.848525;-0.451657;-0.275702;,
    0.892192;-0.451656;-0.000001;,
    0.848525;0.451657;0.275701;,
    0.721798;0.451657;0.524417;,
    0.524417;0.451657;0.721799;,
    0.275702;0.451657;0.848525;,
    -0.000000;0.451657;0.892192;,
    -0.275703;0.451657;0.848525;,
    -0.524417;0.451657;0.721798;,
    -0.721799;0.451657;0.524417;,
    -0.848525;0.451657;0.275702;,
    -0.892192;0.451657;-0.000000;,
    -0.848525;0.451657;-0.275702;,
    -0.721798;0.451657;-0.524417;,
    -0.524417;0.451657;-0.721798;,
    -0.275702;0.451657;-0.848525;,
    0.000000;0.451657;-0.892192;,
    0.275703;0.451657;-0.848525;,
    0.524417;0.451657;-0.721798;,
    0.721798;0.451657;-0.524417;,
    0.848525;0.451657;-0.275702;,
    0.892192;0.451656;-0.000001;,
    0.000000;-1.000000;-0.000000;,
    0.000000;1.000000;-0.000000;,
    0.848525;-0.451657;0.275701;,
    0.848525;0.451657;0.275701;,
    0.000000;-1.000000;-0.000000;,
    0.848525;-0.451657;0.275701;,
    0.000000;-1.000000;-0.000000;,
    0.721798;-0.451657;0.524417;,
    0.000000;-1.000000;-0.000000;,
    0.524417;-0.451657;0.721799;,
    0.000000;-1.000000;-0.000000;,
    0.275702;-0.451657;0.848525;,
    0.000000;-1.000000;-0.000000;,
    -0.000000;-0.451657;0.892192;,
    0.000000;-1.000000;-0.000000;,
    -0.275703;-0.451657;0.848525;,
    0.000000;-1.000000;-0.000000;,
    -0.524417;-0.451657;0.721798;,
    0.000000;-1.000000;-0.000000;,
    -0.721799;-0.451657;0.524417;,
    0.000000;-1.000000;-0.000000;,
    -0.848525;-0.451657;0.275702;,
    0.000000;-1.000000;-0.000000;,
    -0.892192;-0.451657;-0.000000;,
    0.000000;-1.000000;-0.000000;,
    -0.848525;-0.451657;-0.275702;,
    0.000000;-1.000000;-0.000000;,
    -0.721798;-0.451657;-0.524417;,
    0.000000;-1.000000;-0.000000;,
    -0.524417;-0.451657;-0.721798;,
    0.000000;-1.000000;-0.000000;,
    -0.275702;-0.451657;-0.848525;,
    0.000000;-1.000000;-0.000000;,
    0.000000;-0.451657;-0.892192;,
    0.000000;-1.000000;-0.000000;,
    0.275703;-0.451657;-0.848525;,
    0.000000;-1.000000;-0.000000;,
    0.524417;-0.451657;-0.721798;,
    0.000000;-1.000000;-0.000000;,
    0.721798;-0.451657;-0.524417;,
    0.000000;-1.000000;-0.000000;,
    0.848525;-0.451657;-0.275702;,
    0.000000;-1.000000;-0.000000;,
    0.892192;-0.451656;-0.000001;,
    0.000000;1.000000;-0.000000;,
    0.721798;0.451657;0.524417;,
    0.000000;1.000000;-0.000000;,
    0.524417;0.451657;0.721799;,
    0.000000;1.000000;-0.000000;,
    0.275702;0.451657;0.848525;,
    0.000000;1.000000;-0.000000;,
    -0.000000;0.451657;0.892192;,
    0.000000;1.000000;-0.000000;,
    -0.275703;0.451657;0.848525;,
    0.000000;1.000000;-0.000000;,
    -0.524417;0.451657;0.721798;,
    0.000000;1.000000;-0.000000;,
    -0.721799;0.451657;0.524417;,
    0.000000;1.000000;-0.000000;,
    -0.848525;0.451657;0.275702;,
    0.000000;1.000000;-0.000000;,
    -0.892192;0.451657;-0.000000;,
    0.000000;1.000000;-0.000000;,
    -0.848525;0.451657;-0.275702;,
    0.000000;1.000000;-0.000000;,
    -0.721798;0.451657;-0.524417;,
    0.000000;1.000000;-0.000000;,
    -0.524417;0.451657;-0.721798;,
    0.000000;1.000000;-0.000000;,
    -0.275702;0.451657;-0.848525;,
    0.000000;1.000000;-0.000000;,
    0.000000;0.451657;-0.892192;,
    0.000000;1.000000;-0.000000;,
    0.275703;0.451657;-0.848525;,
    0.000000;1.000000;-0.000000;,
    0.524417;0.451657;-0.721798;,
    0.000000;1.000000;-0.000000;,
    0.721798;0.451657;-0.524417;,
    0.000000;1.000000;-0.000000;,
    0.848525;0.451657;-0.275702;,
    0.000000;1.000000;-0.000000;,
    0.892192;0.451656;-0.000001;,
    0.000000;1.000000;-0.000000;,
    0.848525;0.451657;0.275701;;
    80;
    3;21,1,0;,
    3;20,21,0;,
    3;22,2,1;,
    3;21,22,1;,
    3;23,3,2;,
    3;22,23,2;,
    3;24,4,3;,
    3;23,24,3;,
    3;25,5,4;,
    3;24,25,4;,
    3;26,6,5;,
    3;25,26,5;,
    3;27,7,6;,
    3;26,27,6;,
    3;28,8,7;,
    3;27,28,7;,
    3;29,9,8;,
    3;28,29,8;,
    3;30,10,9;,
    3;29,30,9;,
    3;31,11,10;,
    3;30,31,10;,
    3;32,12,11;,
    3;31,32,11;,
    3;33,13,12;,
    3;32,33,12;,
    3;34,14,13;,
    3;33,34,13;,
    3;35,15,14;,
    3;34,35,14;,
    3;36,16,15;,
    3;35,36,15;,
    3;37,17,16;,
    3;36,37,16;,
    3;38,18,17;,
    3;37,38,17;,
    3;39,19,18;,
    3;38,39,18;,
    3;43,42,19;,
    3;39,43,19;,
    3;40,45,44;,
    3;40,47,46;,
    3;40,49,48;,
    3;40,51,50;,
    3;40,53,52;,
    3;40,55,54;,
    3;40,57,56;,
    3;40,59,58;,
    3;40,61,60;,
    3;40,63,62;,
    3;40,65,64;,
    3;40,67,66;,
    3;40,69,68;,
    3;40,71,70;,
    3;40,73,72;,
    3;40,75,74;,
    3;40,77,76;,
    3;40,79,78;,
    3;40,81,80;,
    3;40,83,82;,
    3;41,85,84;,
    3;41,87,86;,
    3;41,89,88;,
    3;41,91,90;,
    3;41,93,92;,
    3;41,95,94;,
    3;41,97,96;,
    3;41,99,98;,
    3;41,101,100;,
    3;41,103,102;,
    3;41,105,104;,
    3;41,107,106;,
    3;41,109,108;,
    3;41,111,110;,
    3;41,113,112;,
    3;41,115,114;,
    3;41,117,116;,
    3;41,119,118;,
    3;41,121,120;,
    3;41,123,122;;
   }
   MeshTextureCoords
   {
    124;
    0.009052;0.890627;,
    0.058147;0.890627;,
    0.107242;0.890627;,
    0.156337;0.890627;,
    0.205431;0.890627;,
    0.254526;0.890627;,
    0.303621;0.890627;,
    0.352716;0.890627;,
    0.401810;0.890627;,
    0.450905;0.890627;,
    0.500000;0.890627;,
    0.549095;0.890627;,
    0.598189;0.890627;,
    0.647284;0.890627;,
    0.696379;0.890627;,
    0.745474;0.890627;,
    0.794568;0.890627;,
    0.843663;0.890627;,
    0.892758;0.890627;,
    0.941853;0.890627;,
    0.009052;0.564564;,
    0.058147;0.564564;,
    0.107242;0.564564;,
    0.156337;0.564564;,
    0.205431;0.564564;,
    0.254526;0.564564;,
    0.303621;0.564564;,
    0.352716;0.564564;,
    0.401810;0.564564;,
    0.450905;0.564564;,
    0.500000;0.564564;,
    0.549095;0.564564;,
    0.598189;0.564564;,
    0.647284;0.564564;,
    0.696379;0.564564;,
    0.745474;0.564564;,
    0.794568;0.564564;,
    0.843663;0.564564;,
    0.892758;0.564564;,
    0.941853;0.564564;,
    0.230631;0.301815;,
    0.735249;0.298511;,
    0.990947;0.890627;,
    0.990947;0.564564;,
    0.383873;0.405575;,
    0.410778;0.352771;,
    0.341968;0.447480;,
    0.383873;0.405575;,
    0.289165;0.474385;,
    0.341968;0.447480;,
    0.230631;0.483656;,
    0.289165;0.474385;,
    0.172098;0.474385;,
    0.230631;0.483656;,
    0.119295;0.447480;,
    0.172098;0.474385;,
    0.077389;0.405575;,
    0.119295;0.447480;,
    0.050485;0.352771;,
    0.077389;0.405575;,
    0.041214;0.294238;,
    0.050485;0.352771;,
    0.050485;0.235705;,
    0.041214;0.294238;,
    0.077389;0.182901;,
    0.050485;0.235705;,
    0.119295;0.140996;,
    0.077389;0.182901;,
    0.172098;0.114091;,
    0.119295;0.140996;,
    0.230631;0.104821;,
    0.172098;0.114091;,
    0.289165;0.114091;,
    0.230631;0.104821;,
    0.341968;0.140996;,
    0.289165;0.114091;,
    0.383873;0.182901;,
    0.341968;0.140996;,
    0.410778;0.235705;,
    0.383873;0.182901;,
    0.420049;0.294238;,
    0.410778;0.235705;,
    0.410778;0.352771;,
    0.420049;0.294238;,
    0.915395;0.232401;,
    0.888491;0.179597;,
    0.888491;0.179597;,
    0.846585;0.137692;,
    0.846585;0.137692;,
    0.793782;0.110788;,
    0.793782;0.110788;,
    0.735249;0.101517;,
    0.735249;0.101517;,
    0.676715;0.110788;,
    0.676715;0.110788;,
    0.623912;0.137692;,
    0.623912;0.137692;,
    0.582007;0.179597;,
    0.582007;0.179597;,
    0.555102;0.232401;,
    0.555102;0.232401;,
    0.545831;0.290934;,
    0.545831;0.290934;,
    0.555102;0.349468;,
    0.555102;0.349468;,
    0.582007;0.402271;,
    0.582007;0.402271;,
    0.623912;0.444176;,
    0.623912;0.444176;,
    0.676715;0.471081;,
    0.676715;0.471081;,
    0.735249;0.480352;,
    0.735249;0.480352;,
    0.793782;0.471081;,
    0.793782;0.471081;,
    0.846586;0.444176;,
    0.846586;0.444176;,
    0.888491;0.402271;,
    0.888491;0.402271;,
    0.915395;0.349468;,
    0.915395;0.349468;,
    0.924666;0.290934;,
    0.924666;0.290934;,
    0.915395;0.232401;;
   }
   MeshMaterialList
   {
    1;
    80;
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
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
    124;
    42;
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
    0,
    20,
    1,
    0,
    2,
    1,
    3,
    2,
    4,
    3,
    5,
    4,
    6,
    5,
    7,
    6,
    8,
    7,
    9,
    8,
    10,
    9,
    11,
    10,
    12,
    11,
    13,
    12,
    14,
    13,
    15,
    14,
    16,
    15,
    17,
    16,
    18,
    17,
    19,
    18,
    0,
    19,
    20,
    21,
    21,
    22,
    22,
    23,
    23,
    24,
    24,
    25,
    25,
    26,
    26,
    27,
    27,
    28,
    28,
    29,
    29,
    30,
    30,
    31,
    31,
    32,
    32,
    33,
    33,
    34,
    34,
    35,
    35,
    36,
    36,
    37,
    37,
    38,
    38,
    39,
    39,
    20;
   }
  }
 }
}

# Animations definition.

