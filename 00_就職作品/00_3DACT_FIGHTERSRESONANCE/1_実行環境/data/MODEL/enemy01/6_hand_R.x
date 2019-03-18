xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 151;
 0.46148;0.53126;-0.60158;,
 0.18172;0.40100;-1.39355;,
 0.03828;1.14525;-1.19869;,
 0.18172;1.34563;-0.47490;,
 -0.40126;0.53126;1.74244;,
 0.18172;0.40100;1.36239;,
 0.03828;1.14525;1.16753;,
 -0.49451;1.34563;1.36239;,
 -0.40126;1.73642;0.57043;,
 0.18172;1.34563;0.44374;,
 -0.83262;1.73642;0.57043;,
 -0.83262;1.34563;1.36239;,
 0.18172;-1.48826;0.44374;,
 0.03828;-1.28789;1.16753;,
 -0.49451;-1.48826;1.36239;,
 -0.40126;-1.87906;0.57043;,
 -0.83262;0.53126;1.74244;,
 -0.40126;0.53126;-1.77359;,
 -0.49451;1.34563;-1.39355;,
 0.03828;-1.28789;-1.19869;,
 0.18172;-1.48826;-0.47490;,
 -0.40126;-1.87906;-0.60158;,
 -0.49451;-1.48826;-1.39355;,
 -0.83262;1.34563;-1.39355;,
 -0.83262;1.73642;-0.60158;,
 -0.40126;1.73642;-0.60158;,
 0.46148;0.53126;0.57043;,
 0.46148;-0.67390;-0.60158;,
 0.46148;-0.67390;0.57043;,
 0.18172;-0.54363;1.36239;,
 -0.40126;-0.67390;-1.77359;,
 0.18172;-0.54363;-1.39355;,
 -0.83262;-1.48826;1.36239;,
 -0.40126;-0.67390;1.74244;,
 -0.83262;-0.67390;1.74244;,
 -0.83262;-0.67390;-1.77359;,
 -0.83262;0.53126;-1.77359;,
 -0.83262;-1.48826;-1.39355;,
 -0.83262;-1.87906;0.57043;,
 -0.83262;-1.87906;-0.60158;,
 -5.37795;1.02705;-2.66498;,
 -7.17304;1.65349;-2.71914;,
 -5.88736;2.04275;-2.43352;,
 -7.17305;1.65349;1.91383;,
 -7.17305;1.65349;2.77748;,
 -5.88737;2.04274;2.79539;,
 -4.71874;2.37948;2.67046;,
 -5.15318;0.59557;3.29697;,
 -1.87793;0.29790;3.73345;,
 -6.28578;-0.31268;1.60089;,
 -4.51036;-0.08019;1.43692;,
 -4.51036;-0.08019;3.19923;,
 -1.87793;2.42973;1.59163;,
 -4.71874;2.37948;1.36323;,
 -1.87793;-1.20331;3.12346;,
 -4.60637;-0.76908;-2.22104;,
 -6.28578;-0.31268;-2.59311;,
 -1.87793;-1.59991;0.05981;,
 -0.52868;-2.21119;0.03195;,
 -0.52868;-1.97115;0.92964;,
 -4.71874;2.37948;-2.55845;,
 -4.71874;2.37948;-1.25122;,
 -1.87793;2.42973;-1.47201;,
 -4.71874;2.37948;0.05600;,
 -1.87793;2.61429;0.05981;,
 -4.51036;-0.08019;-1.34156;,
 -6.28578;-0.31268;-1.54255;,
 -4.51036;-0.08019;0.04768;,
 -6.28578;-0.31268;0.02917;,
 -7.17304;1.65349;-1.85549;,
 -5.88736;2.04274;-1.12629;,
 -7.17304;1.65349;0.02917;,
 -5.88736;2.04274;1.48816;,
 -1.87793;-1.20331;1.59163;,
 -1.87793;-1.20331;-1.47201;,
 -2.54469;-0.98288;-3.27040;,
 -5.88736;2.04274;0.18093;,
 -6.28578;-0.31268;3.17261;,
 -2.77326;0.39363;-3.67725;,
 -5.23382;0.15104;-3.23202;,
 -1.87793;1.79911;3.12346;,
 -0.52868;-0.01658;2.01162;,
 -0.52868;1.49078;1.66369;,
 -0.52868;-1.52395;1.66369;,
 -0.52868;1.93799;-0.86574;,
 -0.52868;1.49078;-1.76343;,
 -0.64502;-1.66532;-1.91440;,
 -0.52868;1.93799;0.92964;,
 -1.87793;1.79911;-3.00383;,
 -0.52868;-0.01658;-2.44520;,
 -0.52868;-2.18036;-0.86574;,
 -0.52868;2.21752;0.03195;,
 -8.29798;0.31343;1.92333;,
 -7.73566;-0.57414;1.37354;,
 -7.73566;-0.57414;0.30679;,
 -8.29267;0.73324;-1.52545;,
 -7.64932;-0.56601;-2.92086;,
 -7.63948;-1.38335;1.85237;,
 -9.50393;-3.17426;2.10744;,
 -9.81460;-2.44988;2.14743;,
 -8.30435;0.32328;3.04825;,
 -7.63905;-1.38511;2.93752;,
 -8.20396;0.76787;1.46485;,
 -9.65192;-1.07130;1.46485;,
 -8.91257;-1.96792;1.37354;,
 -8.20396;0.76787;0.42788;,
 -7.80554;-0.61331;-1.30451;,
 -9.00484;-1.93639;-1.32356;,
 -9.40631;-0.44577;-1.50565;,
 -7.80554;-0.61331;-0.19488;,
 -8.29267;0.73324;-0.19489;,
 -7.64932;-0.56601;-1.79596;,
 -8.84730;-1.19756;-1.79596;,
 -8.06053;1.03928;-1.90389;,
 -8.06053;1.02664;-2.86169;,
 -3.93900;-0.27419;-4.09838;,
 -5.71618;-1.45754;-4.94638;,
 -4.65418;-1.43689;-2.98823;,
 -3.25554;-1.65070;-3.95486;,
 -9.25851;-0.25192;-2.01993;,
 -10.49453;-1.91358;-2.12239;,
 -10.18508;-2.52198;-1.95385;,
 -9.25851;-0.25192;-2.99895;,
 -8.84730;-1.19756;-2.92086;,
 -10.49453;-1.91358;-2.85911;,
 -10.18508;-2.52198;-2.80035;,
 -9.83473;-2.90153;-0.55733;,
 -10.17889;-1.43855;-0.55733;,
 -10.91905;-3.10755;-0.69011;,
 -9.40631;-0.44577;-0.40905;,
 -9.00484;-1.93639;-0.40905;,
 -10.48613;-3.81736;-1.37174;,
 -10.48613;-3.81736;-0.69011;,
 -9.83473;-2.90153;-1.34130;,
 -10.91905;-3.10755;-1.50745;,
 -10.17889;-1.43855;-1.49739;,
 -5.37121;-2.51770;-4.85841;,
 -6.30078;-2.88267;-4.80052;,
 -6.20616;-1.13004;-3.96062;,
 -5.75973;-2.35303;-3.88265;,
 -6.57006;-2.05512;-4.86919;,
 -6.60406;-2.75413;-4.29447;,
 -6.95253;-1.92724;-4.28087;,
 -10.68235;-2.85568;1.31921;,
 -10.13139;-3.52385;1.25116;,
 -8.91257;-1.96792;0.30679;,
 -9.65192;-1.07130;0.42787;,
 -10.68235;-2.85568;0.54645;,
 -10.13139;-3.52385;0.45622;,
 -9.50369;-3.17526;2.71897;,
 -9.81819;-2.44434;2.78137;;
 
 251;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,7,6,9;,
 4;10,11,7,8;,
 4;12,13,14,15;,
 4;16,4,7,11;,
 4;1,17,18,2;,
 4;19,20,21,22;,
 4;20,12,15,21;,
 4;23,24,25,18;,
 4;24,10,8,25;,
 4;18,25,3,2;,
 4;25,8,9,3;,
 4;5,26,9,6;,
 4;26,0,3,9;,
 4;12,20,27,28;,
 4;28,27,0,26;,
 4;13,12,28,29;,
 4;29,28,26,5;,
 4;19,22,30,31;,
 4;31,30,17,1;,
 4;32,14,33,34;,
 4;34,33,4,16;,
 4;14,13,29,33;,
 4;33,29,5,4;,
 4;20,19,31,27;,
 4;27,31,1,0;,
 4;30,35,36,17;,
 4;22,37,35,30;,
 4;21,15,38,39;,
 4;22,21,39,37;,
 4;17,36,23,18;,
 4;15,14,32,38;,
 3;40,41,42;,
 3;43,44,45;,
 3;46,47,48;,
 3;49,50,51;,
 3;52,53,46;,
 3;51,54,48;,
 3;40,55,56;,
 3;57,58,59;,
 3;60,61,62;,
 3;61,63,64;,
 3;64,63,53;,
 3;55,65,66;,
 3;65,67,68;,
 3;68,67,50;,
 3;42,41,69;,
 3;70,69,71;,
 3;71,43,72;,
 3;67,57,73;,
 3;74,57,67;,
 3;75,74,65;,
 3;76,72,53;,
 3;70,76,63;,
 3;42,70,61;,
 3;77,51,47;,
 3;72,45,46;,
 3;73,54,51;,
 3;45,44,47;,
 3;78,40,60;,
 3;55,40,79;,
 3;80,48,81;,
 3;52,80,82;,
 3;54,83,81;,
 3;62,84,85;,
 3;74,75,86;,
 3;64,52,87;,
 3;88,85,89;,
 3;78,89,86;,
 3;57,74,90;,
 3;73,59,83;,
 3;64,91,84;,
 3;86,75,78;,
 3;89,90,86;,
 3;44,43,92;,
 3;49,93,94;,
 3;71,69,95;,
 3;41,56,96;,
 3;97,98,99;,
 3;43,49,97;,
 3;44,100,101;,
 3;77,101,97;,
 3;102,103,104;,
 3;43,102,93;,
 3;71,105,102;,
 3;71,68,94;,
 3;106,107,108;,
 3;69,66,106;,
 3;68,109,106;,
 3;71,110,109;,
 3;96,111,112;,
 3;69,113,111;,
 3;41,114,113;,
 3;56,66,111;,
 3;79,115,116;,
 3;55,117,118;,
 3;40,78,115;,
 3;78,75,118;,
 3;119,120,121;,
 3;114,122,119;,
 3;114,96,123;,
 3;113,119,112;,
 3;120,124,125;,
 3;123,112,121;,
 3;119,122,124;,
 3;123,125,124;,
 3;126,127,128;,
 3;95,108,129;,
 3;109,110,129;,
 3;106,109,130;,
 3;131,132,128;,
 3;133,126,132;,
 3;133,131,134;,
 3;135,134,128;,
 3;108,135,127;,
 3;107,133,135;,
 3;107,130,126;,
 3;130,129,127;,
 3;116,136,137;,
 3;115,118,136;,
 3;117,79,138;,
 3;117,139,136;,
 3;140,137,141;,
 3;138,142,141;,
 3;139,141,137;,
 3;138,116,140;,
 3;103,143,144;,
 3;94,93,104;,
 3;105,94,145;,
 3;102,105,146;,
 3;144,143,147;,
 3;104,144,148;,
 3;146,145,148;,
 3;146,147,143;,
 3;149,150,99;,
 3;92,99,150;,
 3;101,149,98;,
 3;101,100,150;,
 3;40,42,60;,
 3;43,45,72;,
 3;46,48,80;,
 3;49,51,77;,
 3;52,46,80;,
 3;51,48,47;,
 3;40,56,41;,
 3;57,59,73;,
 3;60,62,88;,
 3;61,64,62;,
 3;64,53,52;,
 3;55,66,56;,
 3;65,68,66;,
 3;68,50,49;,
 3;42,69,70;,
 3;70,71,76;,
 3;71,72,76;,
 3;67,73,50;,
 3;74,67,65;,
 3;75,65,55;,
 3;76,53,63;,
 3;70,63,61;,
 3;42,61,60;,
 3;77,47,44;,
 3;72,46,53;,
 3;73,51,50;,
 3;45,47,46;,
 3;78,60,88;,
 3;55,79,117;,
 3;80,81,82;,
 3;52,82,87;,
 3;54,81,48;,
 3;62,85,88;,
 3;74,86,90;,
 3;64,87,91;,
 3;88,89,78;,
 3;57,90,58;,
 3;73,83,54;,
 3;64,84,62;,
 3;58,87,59;,
 3;59,81,83;,
 3;87,82,81;,
 3;59,87,81;,
 3;87,58,91;,
 3;91,58,84;,
 3;84,90,85;,
 3;58,90,84;,
 3;85,90,89;,
 3;44,92,100;,
 3;49,94,68;,
 3;71,95,110;,
 3;41,96,114;,
 3;97,99,92;,
 3;43,97,92;,
 3;44,101,77;,
 3;77,97,49;,
 3;102,104,93;,
 3;43,93,49;,
 3;71,102,43;,
 3;71,94,105;,
 3;106,108,95;,
 3;69,106,95;,
 3;68,106,66;,
 3;71,109,68;,
 3;96,112,123;,
 3;69,111,66;,
 3;41,113,69;,
 3;56,111,96;,
 3;79,116,138;,
 3;55,118,75;,
 3;40,115,79;,
 3;78,118,115;,
 3;119,121,112;,
 3;114,119,113;,
 3;114,123,122;,
 3;113,112,111;,
 3;120,125,121;,
 3;123,121,125;,
 3;119,124,120;,
 3;123,124,122;,
 3;126,128,132;,
 3;95,129,110;,
 3;109,129,130;,
 3;106,130,107;,
 3;131,128,134;,
 3;133,132,131;,
 3;133,134,135;,
 3;135,128,127;,
 3;108,127,129;,
 3;107,135,108;,
 3;107,126,133;,
 3;130,127,126;,
 3;116,137,140;,
 3;115,136,116;,
 3;117,138,139;,
 3;117,136,118;,
 3;140,141,142;,
 3;138,141,139;,
 3;139,137,136;,
 3;138,140,142;,
 3;103,144,104;,
 3;94,104,145;,
 3;105,145,146;,
 3;102,146,103;,
 3;144,147,148;,
 3;104,148,145;,
 3;146,148,147;,
 3;146,143,103;,
 3;149,99,98;,
 3;92,150,100;,
 3;101,98,97;,
 3;101,150,149;;
 
 MeshMaterialList {
  1;
  251;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.640000;0.640000;0.640000;1.000000;;
   0.040000;
   1.000000;1.000000;1.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "DATA\\TEXTURE\\character\\enemy02.png";
   }
  }
 }
 MeshNormals {
  249;
  0.680919;-0.511522;0.524113;,
  0.680919;0.511521;0.524113;,
  0.680918;-0.511522;-0.524115;,
  0.680918;0.511521;-0.524115;,
  0.280072;-0.671122;-0.686408;,
  0.280073;0.671122;-0.686407;,
  0.280074;-0.671123;0.686406;,
  0.280075;0.671124;0.686405;,
  0.779865;-0.597380;-0.186942;,
  0.779865;-0.597380;0.186943;,
  0.779864;0.597381;0.186943;,
  0.779864;0.597382;-0.186942;,
  0.286056;0.932680;-0.219726;,
  0.286057;0.932680;0.219728;,
  0.286059;-0.932679;-0.219728;,
  0.286060;-0.932678;0.219730;,
  0.774182;-0.180498;0.606682;,
  0.774182;0.180497;0.606682;,
  0.774181;0.180496;-0.606684;,
  0.774180;-0.180496;-0.606684;,
  0.280399;0.209669;0.936704;,
  0.280399;-0.209670;0.936704;,
  0.280395;0.209665;-0.936707;,
  0.280395;-0.209666;-0.936706;,
  0.972387;0.162840;0.167172;,
  0.972387;-0.162841;0.167172;,
  0.972387;0.162840;-0.167170;,
  0.972387;-0.162841;-0.167170;,
  0.000000;-0.699370;-0.714759;,
  0.000000;0.699372;-0.714757;,
  0.000000;-0.699372;0.714758;,
  0.000000;0.699374;0.714756;,
  0.000000;-0.973849;-0.227197;,
  0.000000;-0.973848;0.227199;,
  0.000000;0.973850;-0.227192;,
  0.000000;0.973850;0.227194;,
  0.000000;-0.216586;-0.976263;,
  0.000000;0.216584;-0.976264;,
  0.000000;-0.216591;0.976262;,
  0.000000;0.216589;0.976263;,
  0.485166;-0.290460;0.824771;,
  0.489183;0.569882;0.660254;,
  -0.086057;-0.940697;0.328150;,
  0.395327;0.584385;-0.708668;,
  -0.144323;0.018839;0.989351;,
  -0.285479;0.958385;-0.000001;,
  0.136250;-0.062863;-0.988678;,
  -0.010576;0.217128;-0.976086;,
  0.066639;-0.041992;0.996893;,
  0.283814;0.136010;-0.949184;,
  0.120048;0.082459;-0.989338;,
  -0.629122;0.777299;-0.003453;,
  -0.432808;-0.892628;-0.126065;,
  -0.457831;-0.888248;0.037505;,
  -0.367220;-0.919602;0.139577;,
  0.260519;0.934761;0.241561;,
  0.141498;0.989939;-0.000144;,
  0.246817;0.941021;-0.231432;,
  0.231812;-0.972761;-0.000000;,
  0.155061;-0.987905;-0.000000;,
  0.058450;-0.989912;0.129066;,
  -0.283334;0.959021;0.000002;,
  -0.282045;0.959401;0.000000;,
  -0.283333;0.959022;-0.000001;,
  -0.421081;0.907023;0.000001;,
  -0.476273;0.879298;-0.000001;,
  -0.471877;0.881664;-0.000001;,
  0.169751;-0.167128;-0.971212;,
  -0.059551;0.933132;-0.354568;,
  -0.136105;-0.990694;0.000000;,
  -0.070807;0.676602;0.732936;,
  -0.090864;0.111647;0.989585;,
  -0.099660;0.235877;-0.966659;,
  -0.160924;0.986762;-0.020094;,
  -0.200023;0.979791;0.000000;,
  -0.160978;0.986753;0.020095;,
  -0.049117;-0.973646;0.222711;,
  -0.262694;-0.964879;-0.000038;,
  -0.081841;-0.996645;0.000000;,
  -0.342457;-0.880137;0.328758;,
  0.923951;-0.086029;0.372711;,
  0.920218;-0.004489;0.391381;,
  0.781687;0.474852;0.404329;,
  0.376066;0.887249;-0.267139;,
  0.839310;0.438416;-0.321480;,
  0.812705;-0.006138;-0.582642;,
  -0.399973;-0.899633;-0.175165;,
  -0.457287;-0.865719;-0.203517;,
  -0.408662;-0.906755;0.103876;,
  0.392189;0.873684;0.287860;,
  0.282113;0.959381;-0.000477;,
  0.089721;-0.029585;0.995527;,
  -0.021395;-0.016999;0.999627;,
  -0.824726;0.565488;-0.007116;,
  -0.131987;0.017269;-0.991101;,
  0.500337;-0.865831;-0.000000;,
  -0.722071;0.691819;0.000000;,
  -0.052470;-0.087259;0.994803;,
  0.492415;-0.870361;-0.000000;,
  -0.001361;-0.156100;-0.987740;,
  -0.682323;0.731051;0.000000;,
  -0.144897;-0.052411;0.988058;,
  -0.682607;0.730756;-0.006622;,
  0.107879;0.046387;-0.993081;,
  0.280044;-0.959987;-0.000000;,
  -0.116455;0.077673;0.990154;,
  0.070281;0.382392;-0.921323;,
  0.231178;-0.806676;0.543903;,
  0.312797;-0.828382;0.464695;,
  -0.832687;-0.239076;0.499474;,
  -0.767277;0.641316;0.000000;,
  0.030653;-0.041795;-0.998656;,
  0.550733;-0.834681;-0.000000;,
  -0.242463;0.128761;0.961578;,
  -0.891330;-0.453356;0.000000;,
  -0.891330;-0.453356;0.000000;,
  -0.248062;0.129199;0.960090;,
  -0.258701;0.133443;0.956696;,
  0.746745;-0.665110;-0.000000;,
  0.111867;-0.086975;-0.989910;,
  0.111240;-0.101166;-0.988631;,
  -0.748472;0.663166;0.000000;,
  -0.853738;-0.520703;0.000000;,
  -0.853738;-0.520703;0.000000;,
  0.165169;-0.076299;-0.983310;,
  -0.112631;-0.046890;0.992530;,
  -0.121386;-0.042152;0.991710;,
  -0.132104;-0.032680;0.990697;,
  0.796757;-0.604300;-0.000000;,
  -0.878286;0.478136;0.000000;,
  -0.179121;0.395617;-0.900779;,
  0.251106;0.015806;-0.967830;,
  0.350863;-0.876523;0.329549;,
  -0.443223;-0.109204;0.889735;,
  -0.834627;-0.325028;-0.444696;,
  -0.026213;-0.091134;-0.995494;,
  -0.270689;-0.129762;0.953881;,
  0.044750;-0.062416;0.997047;,
  -0.100330;0.048531;-0.993770;,
  -0.827971;0.560771;0.000000;,
  0.030581;-0.076279;0.996617;,
  0.014363;-0.089578;0.995876;,
  0.787226;-0.616665;-0.000000;,
  -0.771532;-0.636190;0.000000;,
  0.014366;-0.089579;0.995876;,
  -0.978275;0.207179;-0.007353;,
  -0.918839;-0.394631;-0.001017;,
  -0.131662;-0.005725;-0.991278;,
  -0.133249;-0.002434;-0.991080;,
  -0.148566;0.988902;-0.000001;,
  -0.094414;-0.266825;0.959109;,
  0.107159;-0.857666;0.502918;,
  -0.283334;0.959021;0.000004;,
  -0.483536;0.875317;-0.003637;,
  -0.312943;-0.949772;0.000000;,
  -0.046365;0.306049;0.950886;,
  -0.081181;0.126505;0.988639;,
  -0.227574;0.186212;-0.955790;,
  -0.908373;-0.304242;0.286870;,
  -0.907299;-0.323306;0.268852;,
  0.785675;0.232882;0.573132;,
  0.703950;-0.213476;-0.677409;,
  -0.236341;-0.935475;0.262733;,
  0.637926;-0.118955;-0.760855;,
  0.998618;-0.030990;-0.042454;,
  -0.132244;0.040355;-0.990395;,
  -0.223525;0.054027;-0.973200;,
  0.472299;-0.881438;-0.000522;,
  0.669511;-0.742800;-0.001472;,
  0.645397;-0.763847;-0.000988;,
  -0.195747;-0.184178;0.963204;,
  -0.112395;-0.139988;0.983753;,
  -0.722072;0.691818;-0.000001;,
  -0.255975;-0.055624;-0.965082;,
  -0.173819;-0.078438;-0.981649;,
  -0.196627;-0.015889;-0.980350;,
  -0.115755;-0.203986;-0.972106;,
  -0.111711;-0.205169;-0.972330;,
  0.492415;-0.870361;-0.000000;,
  -0.143056;-0.057740;0.988029;,
  -0.139998;-0.050488;0.988864;,
  0.375567;-0.926795;-0.000000;,
  -0.140191;0.056596;0.988506;,
  -0.103946;0.057509;0.992919;,
  -0.628988;0.777387;-0.006624;,
  0.035141;-0.999382;-0.000000;,
  -0.379308;0.644716;-0.663677;,
  0.480889;0.020180;-0.876549;,
  -0.767967;0.640485;-0.002249;,
  0.077752;0.025559;-0.996645;,
  -0.167472;0.104517;0.980321;,
  0.630573;-0.776130;-0.000000;,
  0.703549;-0.710646;-0.000000;,
  -0.802364;0.596835;0.000000;,
  0.003665;-0.094286;-0.995538;,
  0.001165;-0.088582;-0.996068;,
  -0.769233;0.638969;0.000000;,
  -0.138305;-0.041486;0.989520;,
  -0.997880;-0.065084;0.000000;,
  0.777853;-0.628446;-0.000000;,
  0.814895;-0.579608;-0.000000;,
  0.147864;-0.077244;-0.985987;,
  0.166851;-0.086029;-0.982222;,
  -0.997880;-0.065084;0.000000;,
  -0.836359;0.548181;0.000000;,
  0.128104;-0.079222;-0.988592;,
  0.752519;-0.658570;-0.000000;,
  -0.138735;-0.035788;0.989683;,
  -0.700514;-0.175528;0.691715;,
  0.328374;-0.875498;0.354505;,
  -0.847885;-0.311497;-0.429023;,
  -0.337362;-0.109542;0.934980;,
  0.243276;-0.895285;0.373206;,
  0.304995;-0.891602;0.334700;,
  -0.530042;0.689879;-0.493074;,
  -0.527910;0.691691;-0.492822;,
  0.775767;-0.631020;-0.000000;,
  -0.166369;0.032529;-0.985527;,
  -0.771532;-0.636190;0.000000;,
  0.787226;-0.616665;-0.000000;,
  -0.122670;0.032795;-0.991906;,
  -0.142233;0.016366;-0.989698;,
  -0.865980;0.500079;0.000000;,
  -0.865980;0.500079;0.000000;,
  -0.993587;-0.112994;-0.004099;,
  0.692639;-0.721283;-0.001451;,
  0.692638;-0.721284;-0.001447;,
  -0.029232;-0.086663;0.995809;,
  1.000000;0.000000;0.000000;,
  -0.824020;0.566479;-0.009627;,
  -0.682323;0.731051;0.000000;,
  -0.085036;0.008666;-0.996340;,
  -0.115103;-0.206590;0.971634;,
  -0.004327;-0.163436;-0.986544;,
  -0.133875;-0.060413;0.989155;,
  -0.194490;0.067981;0.978546;,
  0.068084;-0.052666;-0.996288;,
  -0.891330;-0.453356;0.000000;,
  -0.802364;0.596835;0.000000;,
  -0.108842;-0.066384;0.991840;,
  0.814895;-0.579608;-0.000000;,
  -0.026212;-0.091135;-0.995494;,
  -0.475942;-0.118172;0.871501;,
  -0.820864;-0.338363;-0.460100;,
  -0.527341;0.691849;-0.493211;,
  -0.827971;0.560771;0.000000;,
  -0.142234;0.016366;-0.989698;,
  -0.919049;-0.394144;-0.000285;,
  -0.024821;-0.095634;0.995107;;
  251;
  4;26,18,3,11;,
  4;20,17,1,7;,
  4;13,7,1,10;,
  4;35,31,7,13;,
  4;9,0,6,15;,
  4;39,20,7,31;,
  4;18,22,5,3;,
  4;2,8,14,4;,
  4;8,9,15,14;,
  4;29,34,12,5;,
  4;34,35,13,12;,
  4;5,12,11,3;,
  4;12,13,10,11;,
  4;17,24,10,1;,
  4;24,26,11,10;,
  4;9,8,27,25;,
  4;25,27,26,24;,
  4;0,9,25,16;,
  4;16,25,24,17;,
  4;2,4,23,19;,
  4;19,23,22,18;,
  4;30,6,21,38;,
  4;38,21,20,39;,
  4;6,0,16,21;,
  4;21,16,17,20;,
  4;8,2,19,27;,
  4;27,19,18,26;,
  4;23,36,37,22;,
  4;4,28,36,23;,
  4;14,15,33,32;,
  4;4,14,32,28;,
  4;22,37,29,5;,
  4;15,6,30,33;,
  3;72,50,47;,
  3;66,51,45;,
  3;70,71,48;,
  3;58,78,69;,
  3;55,75,149;,
  3;150,40,48;,
  3;72,67,46;,
  3;53,88,79;,
  3;68,73,57;,
  3;73,74,56;,
  3;56,74,75;,
  3;151,76,60;,
  3;76,77,59;,
  3;59,77,78;,
  3;152,153,64;,
  3;61,64,65;,
  3;65,66,63;,
  3;77,53,54;,
  3;52,53,77;,
  3;42,52,76;,
  3;62,63,75;,
  3;61,62,74;,
  3;152,61,73;,
  3;44,150,71;,
  3;63,45,149;,
  3;54,154,69;,
  3;155,156,71;,
  3;49,72,157;,
  3;158,159,109;,
  3;41,160,81;,
  3;55,41,82;,
  3;40,80,81;,
  3;57,83,84;,
  3;52,42,86;,
  3;56,55,89;,
  3;43,84,85;,
  3;49,85,161;,
  3;53,52,87;,
  3;54,79,162;,
  3;56,90,83;,
  3;161,163,49;,
  3;85,164,161;,
  3;51,66,93;,
  3;58,95,95;,
  3;65,64,100;,
  3;50,46,103;,
  3;94,148,147;,
  3;165,166,94;,
  3;156,91,92;,
  3;167,168,169;,
  3;97,140,137;,
  3;170,97,171;,
  3;65,96,172;,
  3;173,174,175;,
  3;99,119,120;,
  3;176,177,99;,
  3;59,98,178;,
  3;179,101,180;,
  3;181,104,112;,
  3;182,105,183;,
  3;153,102,184;,
  3;185,60,104;,
  3;186,106,130;,
  3;151,108,107;,
  3;72,49,106;,
  3;49,163,187;,
  3;113,117,116;,
  3;102,110,188;,
  3;189,103,111;,
  3;105,113,190;,
  3;115,114,115;,
  3;191,112,192;,
  3;188,110,193;,
  3;111,194,195;,
  3;126,127,125;,
  3;100,196,121;,
  3;180,101,197;,
  3;178,98,118;,
  3;123,122,198;,
  3;128,199,200;,
  3;201,202,124;,
  3;129,203,198;,
  3;196,129,204;,
  3;119,201,205;,
  3;206,118,199;,
  3;207,197,127;,
  3;130,131,135;,
  3;106,187,131;,
  3;208,109,133;,
  3;108,132,209;,
  3;134,210,134;,
  3;133,136,211;,
  3;132,212,213;,
  3;214,130,215;,
  3;140,144,141;,
  3;95,95,216;,
  3;217,175,138;,
  3;172,96,139;,
  3;143,218,143;,
  3;216,219,142;,
  3;220,138,221;,
  3;139,222,223;,
  3;146,145,224;,
  3;93,224,145;,
  3;168,225,226;,
  3;92,91,227;,
  3;72,47,157;,
  3;66,45,63;,
  3;70,48,41;,
  3;58,69,167;,
  3;55,70,41;,
  3;150,48,71;,
  3;72,46,50;,
  3;53,79,54;,
  3;68,57,43;,
  3;73,56,57;,
  3;56,75,55;,
  3;151,60,185;,
  3;76,59,60;,
  3;59,78,58;,
  3;152,64,61;,
  3;61,65,62;,
  3;65,63,62;,
  3;77,54,78;,
  3;52,77,76;,
  3;42,76,151;,
  3;62,75,74;,
  3;61,74,73;,
  3;152,73,68;,
  3;44,71,156;,
  3;63,70,75;,
  3;54,69,78;,
  3;155,71,70;,
  3;49,68,43;,
  3;158,109,208;,
  3;41,81,82;,
  3;55,82,89;,
  3;40,81,48;,
  3;57,84,43;,
  3;52,86,87;,
  3;56,89,90;,
  3;43,85,49;,
  3;53,87,88;,
  3;54,162,154;,
  3;56,83,57;,
  3;228,228,228;,
  3;228,81,80;,
  3;228,82,81;,
  3;228,228,81;,
  3;228,228,228;,
  3;228,228,228;,
  3;228,164,84;,
  3;228,164,228;,
  3;84,164,85;,
  3;51,93,229;,
  3;58,95,59;,
  3;65,100,230;,
  3;50,103,189;,
  3;94,147,231;,
  3;165,94,231;,
  3;156,92,44;,
  3;167,169,58;,
  3;97,137,171;,
  3;170,171,232;,
  3;65,172,66;,
  3;173,175,217;,
  3;99,120,233;,
  3;176,99,233;,
  3;59,178,60;,
  3;179,180,234;,
  3;181,112,191;,
  3;182,183,235;,
  3;153,184,64;,
  3;185,104,181;,
  3;186,130,214;,
  3;151,107,42;,
  3;72,106,186;,
  3;49,187,106;,
  3;113,116,190;,
  3;102,188,184;,
  3;189,111,236;,
  3;105,190,183;,
  3;115,115,237;,
  3;191,192,192;,
  3;188,193,238;,
  3;111,195,236;,
  3;126,125,239;,
  3;100,121,230;,
  3;180,197,207;,
  3;178,118,206;,
  3;123,198,203;,
  3;128,200,240;,
  3;201,124,205;,
  3;129,198,204;,
  3;196,204,121;,
  3;119,205,120;,
  3;206,199,128;,
  3;207,127,126;,
  3;130,135,241;,
  3;106,131,130;,
  3;208,133,242;,
  3;108,209,107;,
  3;134,134,243;,
  3;133,211,242;,
  3;132,213,209;,
  3;214,215,244;,
  3;140,141,137;,
  3;95,216,216;,
  3;217,138,220;,
  3;172,139,245;,
  3;143,143,143;,
  3;216,142,216;,
  3;220,221,246;,
  3;139,223,245;,
  3;146,224,247;,
  3;93,145,229;,
  3;168,226,169;,
  3;92,227,248;;
 }
 MeshTextureCoords {
  151;
  0.166690;0.822260;,
  0.181590;0.831860;,
  0.170850;0.846270;,
  0.156290;0.838640;,
  0.115700;0.787480;,
  0.135920;0.794760;,
  0.129570;0.812740;,
  0.110570;0.817830;,
  0.125310;0.842860;,
  0.141320;0.825860;,
  0.109740;0.859960;,
  0.095460;0.821880;,
  0.170620;0.768650;,
  0.156060;0.761020;,
  0.152120;0.738310;,
  0.175510;0.743720;,
  0.095840;0.779880;,
  0.194310;0.852060;,
  0.174790;0.868980;,
  0.197340;0.794560;,
  0.185590;0.781430;,
  0.201600;0.764440;,
  0.216340;0.789460;,
  0.177820;0.886400;,
  0.146650;0.886400;,
  0.151400;0.863570;,
  0.149030;0.807520;,
  0.177880;0.799780;,
  0.160220;0.785030;,
  0.145320;0.775430;,
  0.211210;0.819820;,
  0.190990;0.812540;,
  0.149100;0.720900;,
  0.132600;0.755230;,
  0.120700;0.736870;,
  0.231070;0.827410;,
  0.206210;0.870420;,
  0.231450;0.785410;,
  0.180260;0.720900;,
  0.217170;0.747330;,
  0.190920;0.785860;,
  0.190640;0.786460;,
  0.190450;0.786330;,
  0.190330;0.787010;,
  0.190250;0.787140;,
  0.190100;0.787040;,
  0.189560;0.787300;,
  0.190020;0.787680;,
  0.188690;0.791260;,
  0.190480;0.787250;,
  0.190840;0.788010;,
  0.190270;0.788270;,
  0.186290;0.787570;,
  0.189500;0.786790;,
  0.190340;0.792100;,
  0.191390;0.786610;,
  0.190820;0.786390;,
  0.193990;0.789530;,
  0.202080;0.795040;,
  0.198590;0.800150;,
  0.190200;0.785610;,
  0.189850;0.785880;,
  0.187620;0.782440;,
  0.189620;0.786310;,
  0.186270;0.784780;,
  0.191460;0.786910;,
  0.190810;0.786700;,
  0.191250;0.787550;,
  0.190660;0.787000;,
  0.190550;0.786620;,
  0.190300;0.786450;,
  0.190430;0.786810;,
  0.190110;0.786840;,
  0.192390;0.791410;,
  0.194380;0.786890;,
  0.194020;0.785430;,
  0.190190;0.786640;,
  0.190270;0.787330;,
  0.192440;0.781600;,
  0.190920;0.785860;,
  0.187380;0.790010;,
  0.185610;0.800980;,
  0.179260;0.794520;,
  0.194030;0.802660;,
  0.181230;0.776070;,
  0.185940;0.771520;,
  0.202550;0.780520;,
  0.177830;0.788510;,
  0.189870;0.780990;,
  0.195910;0.771310;,
  0.204370;0.788980;,
  0.178520;0.781770;,
  0.190330;0.787010;,
  0.190480;0.787250;,
  0.190660;0.787000;,
  0.190550;0.786620;,
  0.190820;0.786390;,
  0.190480;0.787250;,
  0.190480;0.787250;,
  0.190330;0.787010;,
  0.190250;0.787140;,
  0.190270;0.787330;,
  0.190330;0.787010;,
  0.190330;0.787010;,
  0.190480;0.787250;,
  0.190430;0.786810;,
  0.190810;0.786700;,
  0.190810;0.786700;,
  0.190550;0.786620;,
  0.190660;0.787000;,
  0.190430;0.786810;,
  0.190810;0.786700;,
  0.190810;0.786700;,
  0.190550;0.786620;,
  0.190640;0.786460;,
  0.192440;0.781600;,
  0.192440;0.781600;,
  0.191390;0.786610;,
  0.194020;0.785430;,
  0.190550;0.786620;,
  0.190550;0.786620;,
  0.190810;0.786700;,
  0.190640;0.786460;,
  0.190820;0.786390;,
  0.190640;0.786460;,
  0.190820;0.786390;,
  0.190660;0.787000;,
  0.190430;0.786810;,
  0.190430;0.786810;,
  0.190430;0.786810;,
  0.190660;0.787000;,
  0.190810;0.786700;,
  0.190660;0.787000;,
  0.190810;0.786700;,
  0.190550;0.786620;,
  0.190550;0.786620;,
  0.194020;0.785430;,
  0.194020;0.785430;,
  0.190920;0.785860;,
  0.191390;0.786610;,
  0.192440;0.781600;,
  0.191390;0.786610;,
  0.190920;0.785860;,
  0.190330;0.787010;,
  0.190480;0.787250;,
  0.190660;0.787000;,
  0.190430;0.786810;,
  0.190430;0.786810;,
  0.190660;0.787000;,
  0.190270;0.787330;,
  0.190250;0.787140;;
 }
}
