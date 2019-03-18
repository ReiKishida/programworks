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
  "data/TEXTURE/HitomeTexture.jpg";
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
   0.665263,0.000000,0.080941,0.000000,
   0.034827,-0.000000,-0.286244,0.000000,
   0.000000,0.038734,-0.000000,0.000000,
   -14.741626,0.447108,0.000000,1.000000;;
  }
  Mesh
  {
   21;
   -28.716288;-22.638250;-20.000000;,
   18.219795;1.265601;-20.000000;,
   -15.642406;35.198463;-20.000000;,
   21.752260;2.196202;-20.000000;,
   -15.642406;35.198463;20.000000;,
   21.752260;2.196202;20.000000;,
   -28.716288;-22.638250;20.000000;,
   18.219795;1.265601;20.000000;,
   -15.642406;35.198463;-20.000000;,
   -15.642406;35.198463;20.000000;,
   21.752260;2.196202;20.000000;,
   -28.716288;-22.638250;20.000000;,
   18.219795;1.265601;20.000000;,
   18.219795;1.265601;-20.000000;,
   -28.716288;-22.638250;-20.000000;,
   18.219795;1.265601;20.000000;,
   21.752260;2.196202;20.000000;,
   21.752260;2.196202;-20.000000;,
   -28.716288;-22.638250;20.000000;,
   -28.716288;-22.638250;-20.000000;,
   -15.642406;35.198463;20.000000;;
   12;
   3;3,1,0;,
   3;2,3,0;,
   3;5,3,8;,
   3;4,5,8;,
   3;7,10,9;,
   3;6,7,9;,
   3;13,12,11;,
   3;14,13,11;,
   3;16,15,1;,
   3;17,16,1;,
   3;2,19,18;,
   3;20,2,18;;
   MeshNormals
   {
    21;
    0.453821;-0.891093;-0.000000;,
    0.254751;-0.967007;-0.000000;,
    -0.975390;0.220485;-0.000000;,
    0.661700;0.749769;-0.000000;,
    0.661700;0.749769;-0.000000;,
    0.661700;0.749769;-0.000000;,
    0.000000;0.000000;1.000000;,
    0.000000;0.000000;1.000000;,
    0.661700;0.749769;-0.000000;,
    0.000000;0.000000;1.000000;,
    0.000000;0.000000;1.000000;,
    0.453821;-0.891093;-0.000000;,
    0.453821;-0.891093;-0.000000;,
    0.453821;-0.891093;-0.000000;,
    0.453821;-0.891093;-0.000000;,
    0.254751;-0.967007;-0.000000;,
    0.254751;-0.967007;-0.000000;,
    0.254751;-0.967007;-0.000000;,
    -0.975390;0.220485;-0.000000;,
    -0.975390;0.220485;-0.000000;,
    -0.975390;0.220485;-0.000000;;
    12;
    3;3,1,0;,
    3;2,3,0;,
    3;5,3,8;,
    3;4,5,8;,
    3;7,10,9;,
    3;6,7,9;,
    3;13,12,11;,
    3;14,13,11;,
    3;16,15,1;,
    3;17,16,1;,
    3;2,19,18;,
    3;20,2,18;;
   }
   MeshTextureCoords
   {
    21;
    0.104021;0.437380;,
    0.310645;0.437380;,
    0.425475;0.523213;,
    0.281398;0.154080;,
    0.304618;0.490002;,
    0.221712;0.121560;,
    0.070302;0.121406;,
    0.229162;0.121406;,
    0.425475;0.523213;,
    0.304618;0.490002;,
    0.221712;0.121560;,
    0.070302;0.121406;,
    0.229162;0.121406;,
    0.188261;0.039762;,
    0.051522;0.039762;,
    0.443278;0.437380;,
    0.424002;0.140689;,
    0.281398;0.154080;,
    0.062800;0.437380;,
    0.104021;0.437380;,
    0.252879;0.504879;;
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
    21;
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
    4,
    5,
    6,
    7,
    1,
    0,
    7,
    5,
    3,
    6,
    0,
    4;
   }
  }
 }
}

# Animations definition.

