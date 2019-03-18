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
  "data/TEXTURE/object/box.jpg";
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
   1.435180,0.000000,0.000000,0.000000,
   0.000000,1.000000,0.000000,0.000000,
   0.000000,0.000000,1.000000,0.000000,
   0.000000,203.064011,-20.796700,1.000000;;
  }
  Mesh
  {
   24;
   -200.000000;-200.000000;-200.000000;,
   200.000000;-200.000000;-200.000000;,
   -200.000000;200.000000;-200.000000;,
   200.000000;200.000000;-200.000000;,
   -200.000000;200.000000;200.000000;,
   200.000000;200.000000;200.000000;,
   -200.000000;-200.000000;200.000000;,
   200.000000;-200.000000;200.000000;,
   -200.000000;200.000000;-200.000000;,
   200.000000;200.000000;-200.000000;,
   -200.000000;200.000000;200.000000;,
   200.000000;200.000000;200.000000;,
   -200.000000;-200.000000;200.000000;,
   200.000000;-200.000000;200.000000;,
   200.000000;-200.000000;-200.000000;,
   -200.000000;-200.000000;-200.000000;,
   200.000000;-200.000000;-200.000000;,
   200.000000;-200.000000;200.000000;,
   200.000000;200.000000;200.000000;,
   200.000000;200.000000;-200.000000;,
   -200.000000;-200.000000;200.000000;,
   -200.000000;-200.000000;-200.000000;,
   -200.000000;200.000000;-200.000000;,
   -200.000000;200.000000;200.000000;;
   12;
   3;3,1,0;,
   3;2,3,0;,
   3;5,9,8;,
   3;4,5,8;,
   3;7,11,10;,
   3;6,7,10;,
   3;14,13,12;,
   3;15,14,12;,
   3;18,17,16;,
   3;19,18,16;,
   3;22,21,20;,
   3;23,22,20;;
   MeshNormals
   {
    24;
    0.000000;0.000000;-1.000000;,
    0.000000;0.000000;-1.000000;,
    0.000000;0.000000;-1.000000;,
    0.000000;0.000000;-1.000000;,
    0.000000;1.000000;-0.000000;,
    0.000000;1.000000;-0.000000;,
    0.000000;0.000000;1.000000;,
    0.000000;0.000000;1.000000;,
    0.000000;1.000000;-0.000000;,
    0.000000;1.000000;-0.000000;,
    0.000000;0.000000;1.000000;,
    0.000000;0.000000;1.000000;,
    0.000000;-1.000000;-0.000000;,
    0.000000;-1.000000;-0.000000;,
    0.000000;-1.000000;-0.000000;,
    0.000000;-1.000000;-0.000000;,
    1.000000;0.000000;-0.000000;,
    1.000000;0.000000;-0.000000;,
    1.000000;0.000000;-0.000000;,
    1.000000;0.000000;-0.000000;,
    -1.000000;0.000000;-0.000000;,
    -1.000000;0.000000;-0.000000;,
    -1.000000;0.000000;-0.000000;,
    -1.000000;0.000000;-0.000000;;
    12;
    3;3,1,0;,
    3;2,3,0;,
    3;5,9,8;,
    3;4,5,8;,
    3;7,11,10;,
    3;6,7,10;,
    3;14,13,12;,
    3;15,14,12;,
    3;18,17,16;,
    3;19,18,16;,
    3;22,21,20;,
    3;23,22,20;;
   }
   MeshTextureCoords
   {
    24;
    -0.023867;1.035700;,
    1.038250;1.035700;,
    -0.023867;0.493770;,
    1.038250;0.493770;,
    0.494027;0.015815;,
    0.994966;0.015815;,
    1.038650;1.034530;,
    -0.014791;1.038890;,
    0.494027;0.528862;,
    0.994966;0.528862;,
    1.030200;0.491520;,
    -0.023243;0.495877;,
    0.683473;0.684976;,
    0.837125;0.684976;,
    0.837125;0.531325;,
    0.683473;0.531325;,
    0.422848;0.514525;,
    0.420686;0.025213;,
    -0.068626;0.027375;,
    -0.066464;0.516686;,
    0.058862;0.030422;,
    0.057413;0.510028;,
    0.537019;0.511477;,
    0.538468;0.031871;;
   }
   MeshMaterialList
   {
    1;
    12;
    0,
    0,
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
    24;
    8;
    0,
    1,
    2,
    3,
    4,
    5,
    6,
    7,
    2,
    3,
    4,
    5,
    6,
    7,
    1,
    0,
    1,
    7,
    5,
    3,
    6,
    0,
    2,
    4;
   }
  }
 }
}

# Animations definition.

