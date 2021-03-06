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
  "data/TEXTURE/kamiko_texx.jpg";
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
   0.000000,-5.897710,-0.000000,1.000000;;
  }
  Mesh
  {
   34;
   -17.573284;4.984833;-1.058513;,
   -17.573284;6.585783;-1.058513;,
   -0.016947;4.544296;-1.499050;,
   -0.016947;7.026312;-1.499050;,
   -0.016947;4.544296;0.982970;,
   -0.016947;7.026312;0.982970;,
   -17.573284;4.984833;0.542433;,
   -17.573284;6.585783;0.542433;,
   -4.403948;7.021240;-1.493971;,
   0.702261;5.785308;-1.933400;,
   0.702261;4.109947;-0.258038;,
   0.702261;7.460668;-0.258038;,
   0.702261;5.785308;1.417322;,
   -4.403948;4.549375;0.977895;,
   -4.403948;7.021240;0.977895;,
   -18.037182;5.785308;0.822600;,
   -18.037182;4.704674;-0.258038;,
   -18.037182;6.865950;-0.258038;,
   -18.037182;5.785308;-1.338676;,
   -4.403948;4.549375;-1.493971;,
   -4.403948;5.785308;-1.905946;,
   1.626957;5.785308;-0.258038;,
   -4.403948;5.785308;1.389874;,
   -18.633631;5.785308;-0.258038;,
   -4.403948;7.433215;-0.258038;,
   -4.403948;4.137400;-0.258038;,
   -8.486853;4.669531;-1.373811;,
   -8.486856;5.785308;-1.754777;,
   -8.486853;6.901084;-1.373811;,
   -8.486856;7.282051;-0.258038;,
   -8.486853;6.901084;0.857734;,
   -8.486856;5.785308;1.238705;,
   -8.486853;4.669531;0.857734;,
   -8.486856;4.288573;-0.258038;;
   64;
   3;20,19,26;,
   3;27,20,26;,
   3;8,20,27;,
   3;28,8,27;,
   3;3,9,20;,
   3;8,3,20;,
   3;9,2,19;,
   3;20,9,19;,
   3;21,10,2;,
   3;9,21,2;,
   3;11,21,9;,
   3;3,11,9;,
   3;5,12,21;,
   3;11,5,21;,
   3;12,4,10;,
   3;21,12,10;,
   3;22,13,4;,
   3;12,22,4;,
   3;14,22,12;,
   3;5,14,12;,
   3;30,31,22;,
   3;14,30,22;,
   3;31,32,13;,
   3;22,31,13;,
   3;23,16,6;,
   3;15,23,6;,
   3;17,23,15;,
   3;7,17,15;,
   3;1,18,23;,
   3;17,1,23;,
   3;18,0,16;,
   3;23,18,16;,
   3;24,8,28;,
   3;29,24,28;,
   3;14,24,29;,
   3;30,14,29;,
   3;5,11,24;,
   3;14,5,24;,
   3;11,3,8;,
   3;24,11,8;,
   3;25,13,32;,
   3;33,25,32;,
   3;19,25,33;,
   3;26,19,33;,
   3;2,10,25;,
   3;19,2,25;,
   3;10,4,13;,
   3;25,10,13;,
   3;27,26,0;,
   3;18,27,0;,
   3;28,27,18;,
   3;1,28,18;,
   3;29,28,1;,
   3;17,29,1;,
   3;30,29,17;,
   3;7,30,17;,
   3;7,15,31;,
   3;30,7,31;,
   3;15,6,32;,
   3;31,15,32;,
   3;33,32,6;,
   3;16,33,6;,
   3;26,33,16;,
   3;0,26,16;;
   MeshNormals
   {
    34;
    -0.301845;-0.673864;-0.674387;,
    -0.311045;0.672294;-0.671767;,
    0.280159;-0.679032;-0.678547;,
    0.270896;0.680426;-0.680908;,
    0.270896;-0.680426;0.680907;,
    0.280158;0.679032;0.678547;,
    -0.311045;-0.672297;0.671764;,
    -0.301844;0.673862;0.674389;,
    -0.021995;0.706761;-0.707110;,
    0.446165;0.004277;-0.894940;,
    0.446164;-0.894938;0.004858;,
    0.446164;0.894938;-0.004858;,
    0.446166;-0.004276;0.894940;,
    -0.021996;-0.706763;0.707108;,
    -0.021995;0.707110;0.706761;,
    -0.470225;0.004215;0.882537;,
    -0.470223;-0.882534;-0.004832;,
    -0.470224;0.882534;0.004833;,
    -0.470225;-0.004215;-0.882536;,
    -0.021996;-0.707110;-0.706761;,
    -0.020827;-0.000247;-0.999783;,
    1.000000;0.000001;0.000000;,
    -0.020826;0.000247;0.999783;,
    -1.000000;-0.000001;0.000001;,
    -0.020826;0.999783;-0.000247;,
    -0.020827;-0.999783;0.000247;,
    -0.043776;-0.706238;-0.706620;,
    -0.041732;0.000265;-0.999129;,
    -0.043776;0.706621;-0.706237;,
    -0.041732;0.999129;0.000265;,
    -0.043776;0.706237;0.706621;,
    -0.041733;-0.000265;0.999129;,
    -0.043776;-0.706624;0.706234;,
    -0.041733;-0.999129;-0.000264;;
    64;
    3;20,19,26;,
    3;27,20,26;,
    3;8,20,27;,
    3;28,8,27;,
    3;3,9,20;,
    3;8,3,20;,
    3;9,2,19;,
    3;20,9,19;,
    3;21,10,2;,
    3;9,21,2;,
    3;11,21,9;,
    3;3,11,9;,
    3;5,12,21;,
    3;11,5,21;,
    3;12,4,10;,
    3;21,12,10;,
    3;22,13,4;,
    3;12,22,4;,
    3;14,22,12;,
    3;5,14,12;,
    3;30,31,22;,
    3;14,30,22;,
    3;31,32,13;,
    3;22,31,13;,
    3;23,16,6;,
    3;15,23,6;,
    3;17,23,15;,
    3;7,17,15;,
    3;1,18,23;,
    3;17,1,23;,
    3;18,0,16;,
    3;23,18,16;,
    3;24,8,28;,
    3;29,24,28;,
    3;14,24,29;,
    3;30,14,29;,
    3;5,11,24;,
    3;14,5,24;,
    3;11,3,8;,
    3;24,11,8;,
    3;25,13,32;,
    3;33,25,32;,
    3;19,25,33;,
    3;26,19,33;,
    3;2,10,25;,
    3;19,2,25;,
    3;10,4,13;,
    3;25,10,13;,
    3;27,26,0;,
    3;18,27,0;,
    3;28,27,18;,
    3;1,28,18;,
    3;29,28,1;,
    3;17,29,1;,
    3;30,29,17;,
    3;7,30,17;,
    3;7,15,31;,
    3;30,7,31;,
    3;15,6,32;,
    3;31,15,32;,
    3;33,32,6;,
    3;16,33,6;,
    3;26,33,16;,
    3;0,26,16;;
   }
   MeshTextureCoords
   {
    34;
    0.335627;0.340945;,
    0.335627;0.320955;,
    0.371881;0.346445;,
    0.371881;0.315454;,
    0.371881;0.346445;,
    0.371881;0.315454;,
    0.335627;0.340945;,
    0.335627;0.320955;,
    0.362822;0.315517;,
    0.373366;0.330950;,
    0.373366;0.351869;,
    0.373366;0.310030;,
    0.373366;0.330950;,
    0.362822;0.346382;,
    0.362822;0.315517;,
    0.334669;0.330950;,
    0.334669;0.344443;,
    0.334669;0.317456;,
    0.334669;0.330950;,
    0.362822;0.346382;,
    0.362822;0.330950;,
    0.375276;0.330950;,
    0.362822;0.330950;,
    0.333437;0.330950;,
    0.362822;0.310373;,
    0.362822;0.351526;,
    0.354390;0.344882;,
    0.354390;0.330950;,
    0.354390;0.317018;,
    0.354390;0.312261;,
    0.354390;0.317018;,
    0.354390;0.330950;,
    0.354390;0.344882;,
    0.354390;0.349638;;
   }
   MeshMaterialList
   {
    1;
    64;
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
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
    34;
    34;
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
    33;
   }
  }
 }
}

# Animations definition.

