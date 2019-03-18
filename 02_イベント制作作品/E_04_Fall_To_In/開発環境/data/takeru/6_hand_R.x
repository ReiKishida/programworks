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
 179;
 0.36537;0.30970;-0.26697;,
 0.18701;0.25025;-0.63865;,
 0.09557;0.58992;-0.54720;,
 0.18701;0.68137;-0.20752;,
 -0.18466;0.30970;0.83309;,
 0.18701;0.25025;0.65473;,
 0.09556;0.58992;0.56328;,
 -0.24411;0.68137;0.65473;,
 -0.18466;0.85973;0.28306;,
 0.18701;0.68137;0.22360;,
 -0.45968;0.85973;0.28306;,
 -0.45968;0.68137;0.65473;,
 0.18701;-0.61201;0.22360;,
 0.09556;-0.52056;0.56328;,
 -0.24411;-0.61201;0.65473;,
 -0.18466;-0.79036;0.28306;,
 -0.45968;0.30970;0.83309;,
 -0.18466;0.30970;-0.81701;,
 -0.24411;0.68137;-0.63865;,
 0.09557;-0.52056;-0.54720;,
 0.18701;-0.61201;-0.20752;,
 -0.18466;-0.79036;-0.26697;,
 -0.24411;-0.61201;-0.63865;,
 -0.45967;0.68137;-0.63865;,
 -0.45967;0.85973;-0.26697;,
 -0.18466;0.85973;-0.26697;,
 0.36537;0.30970;0.28306;,
 0.36537;-0.24033;-0.26697;,
 0.36537;-0.24033;0.28306;,
 0.18701;-0.18088;0.65473;,
 -0.18466;-0.24033;-0.81701;,
 0.18701;-0.18088;-0.63865;,
 -0.45967;-0.61201;0.65473;,
 -0.18466;-0.24033;0.83309;,
 -0.45968;-0.24033;0.83309;,
 -0.45967;-0.24033;-0.81701;,
 -0.45967;0.30970;-0.81701;,
 -0.45967;-0.61201;-0.63865;,
 -0.45967;-0.79036;0.28306;,
 -0.45967;-0.79036;-0.26697;,
 -3.18888;0.38584;-1.86308;,
 -4.78986;0.25150;-1.71631;,
 -4.17151;1.11671;-1.16912;,
 -4.78986;0.25150;0.90175;,
 -4.78986;0.25150;1.77444;,
 -4.17151;1.11671;1.25211;,
 -2.91560;1.18916;1.25211;,
 -3.18888;0.38584;1.58037;,
 -1.12861;0.25180;1.74432;,
 -3.90133;-0.02315;0.75685;,
 -2.78452;0.08154;0.68092;,
 -2.78452;0.08154;1.32421;,
 -1.12861;1.21178;0.75256;,
 -2.91560;1.18916;0.64680;,
 -1.12861;-0.42421;1.46187;,
 -4.26637;-0.87743;0.89779;,
 -4.77981;-0.50077;0.99948;,
 -2.91920;0.00884;-0.94596;,
 -3.90133;-0.02315;-1.42649;,
 -1.12861;-0.60281;0.04326;,
 -0.27987;-0.87807;0.03036;,
 -0.27987;-0.76998;0.44603;,
 -4.78986;0.25150;-0.84362;,
 -4.80992;-0.47341;-1.57430;,
 -3.90133;-0.02315;-0.69871;,
 -4.23300;-0.88932;-0.62622;,
 -4.80279;-0.47131;-0.73907;,
 -4.80809;-0.46763;0.80395;,
 -2.91560;1.18916;-1.16912;,
 -2.91560;1.18916;-0.56381;,
 -1.12861;1.21178;-0.66605;,
 -2.91560;1.18916;0.04149;,
 -1.12861;1.29489;0.04326;,
 -2.78452;0.08154;-0.60564;,
 -2.78452;0.08154;0.03764;,
 -3.90133;-0.02315;0.02907;,
 -4.17151;1.11671;-0.56382;,
 -4.78986;0.25150;0.02907;,
 -4.17151;1.11671;0.64680;,
 -4.23361;-1.39153;0.89779;,
 -4.74705;-1.01487;0.99948;,
 -4.77981;-0.50077;1.61188;,
 -3.90133;-0.02315;1.48463;,
 -4.26637;-0.87743;1.40851;,
 -4.80809;-0.46763;0.11188;,
 -4.79154;-0.96409;0.11188;,
 -4.22786;-0.89329;0.11188;,
 -4.22786;-0.89329;0.68903;,
 -4.24522;-1.35218;-0.62622;,
 -4.81500;-0.93418;-0.73907;,
 -4.23300;-0.88932;-0.05945;,
 -4.80279;-0.47131;-0.05945;,
 -4.23191;-0.89744;-0.79796;,
 -4.80992;-0.47341;-0.93998;,
 -4.86268;-1.00501;-0.93998;,
 -4.23191;-0.89744;-1.37290;,
 -4.23361;-1.39153;1.40851;,
 -3.78252;-1.54684;0.89779;,
 -4.74705;-1.01487;1.61188;,
 -4.21132;-1.38975;0.11188;,
 -4.21132;-1.38975;0.68903;,
 -3.73166;-1.53212;0.11188;,
 -4.79155;-0.96409;0.80395;,
 -4.24522;-1.35218;-0.05945;,
 -4.81500;-0.93418;-0.05945;,
 -3.41087;-1.08192;-0.55362;,
 -4.28467;-1.42904;-1.37290;,
 -4.28467;-1.42904;-0.79796;,
 -3.48504;-1.09727;-1.24721;,
 -3.51473;-1.39644;-1.24721;,
 -4.86268;-1.00501;-1.57430;,
 -3.81528;-1.03274;0.89779;,
 -3.81528;-1.03274;1.40851;,
 -3.50852;-1.04064;1.35019;,
 -3.78252;-1.54684;1.40851;,
 -3.48744;-1.40421;1.35019;,
 -3.18786;-1.30449;1.25783;,
 -3.48744;-1.40421;0.95611;,
 -3.50852;-1.04064;0.95611;,
 -3.74821;-1.03565;0.11188;,
 -3.41891;-1.11394;0.20781;,
 -3.73166;-1.53212;0.68903;,
 -3.74821;-1.03565;0.68903;,
 -3.40787;-1.44537;0.59310;,
 -3.27005;-1.37214;0.51008;,
 -3.27633;-1.18354;0.51008;,
 -3.41891;-1.11394;0.59310;,
 -3.40787;-1.44537;0.20781;,
 -3.27633;-1.18354;0.29083;,
 -3.27005;-1.37214;0.29083;,
 -3.20129;-1.09374;1.04847;,
 -3.20129;-1.09374;1.25783;,
 -3.18786;-1.30449;1.04847;,
 -3.41995;-1.42619;-0.13206;,
 -3.09291;-1.40435;-0.18255;,
 -3.08600;-1.14254;-0.18254;,
 -3.41087;-1.08192;-0.13206;,
 -3.41995;-1.42619;-0.55362;,
 -3.08600;-1.14254;-0.50313;,
 -3.09291;-1.40435;-0.50313;,
 -3.48504;-1.09727;-0.92364;,
 -3.51473;-1.39644;-0.92364;,
 -3.04213;-1.37433;-0.99586;,
 -3.02569;-1.20870;-1.17499;,
 -3.02569;-1.20870;-0.99586;,
 -3.04213;-1.37433;-1.17499;,
 -1.12861;-0.42421;0.75256;,
 -1.12861;-0.42421;-0.66605;,
 -1.24689;-0.63985;-1.06966;,
 -4.17151;1.11671;0.04149;,
 -2.52619;-1.20337;-1.36413;,
 -2.43075;-1.22690;-0.69315;,
 -1.12861;0.92781;-1.37535;,
 -1.12861;0.25180;-2.10782;,
 -2.00506;-1.45820;-0.95356;,
 -2.73202;-2.19829;-0.70354;,
 -2.71744;-2.10854;-1.56289;,
 -2.49134;-1.60350;-1.77532;,
 -2.99436;-1.52390;-1.30688;,
 -3.38412;-2.02033;-1.46213;,
 -3.00514;-1.55350;-0.66598;,
 -3.61959;-1.53187;-0.69997;,
 -3.59680;-1.50629;-1.24761;,
 -3.93327;-1.45075;-1.20456;,
 -3.41717;-2.09727;-0.72777;,
 -3.77151;-1.98296;-0.73641;,
 -3.95380;-1.47378;-0.71138;,
 -3.79474;-1.91367;-1.38847;,
 -1.12861;0.92781;1.46187;,
 -0.27987;0.11018;0.94703;,
 -0.27987;0.78897;0.78593;,
 -0.27987;-0.56860;0.78593;,
 -0.27987;0.99035;-0.38532;,
 -0.27987;0.78897;-0.80099;,
 -0.35305;-0.69023;-0.87089;,
 -0.27987;0.99035;0.44603;,
 -0.27987;0.11018;-1.11668;,
 -0.27987;-0.86419;-0.38532;,
 -0.27987;1.11622;0.03036;;
 
 307;
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
 3;49,55,56;,
 3;40,57,58;,
 3;59,60,61;,
 3;62,41,63;,
 3;64,65,66;,
 3;49,43,67;,
 3;68,69,70;,
 3;69,71,72;,
 3;72,71,53;,
 3;57,73,64;,
 3;73,74,75;,
 3;75,74,50;,
 3;42,41,62;,
 3;76,62,77;,
 3;77,43,78;,
 3;55,79,80;,
 3;43,56,81;,
 3;82,83,55;,
 3;82,44,81;,
 3;67,84,85;,
 3;75,86,84;,
 3;49,87,86;,
 3;77,84,67;,
 3;65,88,89;,
 3;64,75,90;,
 3;77,62,66;,
 3;75,77,91;,
 3;92,93,94;,
 3;64,62,93;,
 3;58,95,63;,
 3;58,64,92;,
 3;80,79,96;,
 3;81,56,80;,
 3;96,79,97;,
 3;83,81,98;,
 3;85,99,100;,
 3;86,99,85;,
 3;100,99,101;,
 3;87,67,102;,
 3;88,103,104;,
 3;88,65,105;,
 3;90,91,104;,
 3;91,66,89;,
 3;106,107,94;,
 3;95,108,109;,
 3;93,63,110;,
 3;95,106,110;,
 3;111,112,113;,
 3;96,114,112;,
 3;79,55,111;,
 3;83,112,111;,
 3;113,115,116;,
 3;112,114,115;,
 3;97,117,115;,
 3;111,118,117;,
 3;101,119,120;,
 3;100,121,122;,
 3;86,87,122;,
 3;99,86,119;,
 3;123,124,125;,
 3;122,126,120;,
 3;121,123,126;,
 3;101,127,123;,
 3;128,125,124;,
 3;127,129,124;,
 3;127,120,128;,
 3;120,126,125;,
 3;130,131,116;,
 3;115,117,132;,
 3;118,130,132;,
 3;113,131,130;,
 3;133,134,135;,
 3;90,136,105;,
 3;103,133,136;,
 3;103,88,137;,
 3;138,135,134;,
 3;137,139,134;,
 3;137,105,138;,
 3;105,136,135;,
 3;140,141,142;,
 3;92,107,141;,
 3;95,92,140;,
 3;107,106,109;,
 3;143,144,142;,
 3;108,140,144;,
 3;108,143,145;,
 3;141,109,145;,
 3;74,59,146;,
 3;147,59,74;,
 3;148,147,73;,
 3;71,149,78;,
 3;69,76,149;,
 3;68,42,76;,
 3;57,150,151;,
 3;82,51,47;,
 3;53,78,45;,
 3;146,54,51;,
 3;45,44,47;,
 3;152,153,40;,
 3;154,155,156;,
 3;40,157,150;,
 3;153,154,157;,
 3;148,151,154;,
 3;158,156,159;,
 3;151,160,155;,
 3;157,156,158;,
 3;151,150,158;,
 3;161,162,163;,
 3;160,158,162;,
 3;155,164,159;,
 3;160,161,164;,
 3;165,166,163;,
 3;164,165,167;,
 3;161,166,165;,
 3;162,159,167;,
 3;168,48,169;,
 3;52,168,170;,
 3;54,171,169;,
 3;70,172,173;,
 3;147,148,174;,
 3;72,52,175;,
 3;152,173,176;,
 3;153,176,174;,
 3;59,147,177;,
 3;146,61,171;,
 3;72,178,172;,
 3;174,148,153;,
 3;176,177,174;,
 3;40,42,68;,
 3;43,45,78;,
 3;46,48,168;,
 3;49,51,82;,
 3;52,46,168;,
 3;51,48,47;,
 3;49,56,43;,
 3;40,58,41;,
 3;59,61,146;,
 3;62,63,93;,
 3;64,66,62;,
 3;49,67,87;,
 3;68,70,152;,
 3;69,72,70;,
 3;72,53,52;,
 3;57,64,58;,
 3;73,75,64;,
 3;75,50,49;,
 3;42,62,76;,
 3;76,77,149;,
 3;77,78,149;,
 3;55,80,56;,
 3;43,81,44;,
 3;82,55,49;,
 3;82,81,83;,
 3;67,85,102;,
 3;75,84,77;,
 3;49,86,75;,
 3;77,67,43;,
 3;65,89,66;,
 3;64,90,65;,
 3;77,66,91;,
 3;75,91,90;,
 3;92,94,107;,
 3;64,93,92;,
 3;58,63,41;,
 3;58,92,95;,
 3;80,96,98;,
 3;81,80,98;,
 3;96,97,114;,
 3;83,98,96;,
 3;85,100,102;,
 3;86,85,84;,
 3;100,101,121;,
 3;87,102,100;,
 3;88,104,89;,
 3;88,105,137;,
 3;90,104,103;,
 3;91,89,104;,
 3;106,94,110;,
 3;95,109,106;,
 3;93,110,94;,
 3;95,110,63;,
 3;111,113,118;,
 3;96,112,83;,
 3;79,111,97;,
 3;83,111,55;,
 3;113,116,131;,
 3;112,115,113;,
 3;97,115,114;,
 3;111,117,97;,
 3;101,120,127;,
 3;100,122,87;,
 3;86,122,119;,
 3;99,119,101;,
 3;123,125,126;,
 3;122,120,119;,
 3;121,126,122;,
 3;101,123,121;,
 3;128,124,129;,
 3;127,124,123;,
 3;127,128,129;,
 3;120,125,128;,
 3;130,116,132;,
 3;115,132,116;,
 3;118,132,117;,
 3;113,130,118;,
 3;133,135,136;,
 3;90,105,65;,
 3;103,136,90;,
 3;103,137,133;,
 3;138,134,139;,
 3;137,134,133;,
 3;137,138,139;,
 3;105,135,138;,
 3;140,142,144;,
 3;92,141,140;,
 3;95,140,108;,
 3;107,109,141;,
 3;143,142,145;,
 3;108,144,143;,
 3;108,145,109;,
 3;141,145,142;,
 3;74,146,50;,
 3;147,74,73;,
 3;148,73,57;,
 3;71,78,53;,
 3;69,149,71;,
 3;68,76,69;,
 3;57,151,148;,
 3;82,47,44;,
 3;53,45,46;,
 3;146,51,50;,
 3;45,47,46;,
 3;152,40,68;,
 3;154,156,157;,
 3;40,150,57;,
 3;153,157,40;,
 3;148,154,153;,
 3;158,159,162;,
 3;151,155,154;,
 3;157,158,150;,
 3;151,158,160;,
 3;161,163,166;,
 3;160,162,161;,
 3;155,159,156;,
 3;160,164,155;,
 3;165,163,167;,
 3;164,167,159;,
 3;161,165,164;,
 3;162,167,163;,
 3;168,169,170;,
 3;52,170,175;,
 3;54,169,48;,
 3;70,173,152;,
 3;147,174,177;,
 3;72,175,178;,
 3;152,176,153;,
 3;59,177,60;,
 3;146,171,54;,
 3;72,172,70;,
 3;60,175,61;,
 3;61,169,171;,
 3;175,170,169;,
 3;61,175,169;,
 3;175,60,178;,
 3;178,60,172;,
 3;172,177,173;,
 3;60,177,172;,
 3;173,177,176;;
 
 MeshMaterialList {
  1;
  307;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
   0.500000;
   1.000000;1.000000;1.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\enemy_1.png";
   }
  }
 }
 MeshNormals {
  297;
  0.577352;-0.577351;0.577348;,
  0.577352;0.577352;0.577347;,
  0.577356;-0.577351;-0.577344;,
  0.577356;0.577352;-0.577343;,
  0.214806;-0.690607;-0.690594;,
  0.214807;0.690604;-0.690597;,
  0.214809;-0.690604;0.690597;,
  0.214810;0.690600;0.690599;,
  0.690600;-0.690603;-0.214798;,
  0.690598;-0.690604;0.214805;,
  0.690600;0.690601;0.214807;,
  0.690603;0.690600;-0.214800;,
  0.217714;0.951422;-0.217708;,
  0.217716;0.951420;0.217714;,
  0.217705;-0.951426;-0.217700;,
  0.217708;-0.951424;0.217705;,
  0.690601;-0.214805;0.690600;,
  0.690600;0.214810;0.690599;,
  0.690606;0.214805;-0.690595;,
  0.690607;-0.214800;-0.690596;,
  0.217715;0.217715;0.951420;,
  0.217715;-0.217710;0.951421;,
  0.217714;0.217714;-0.951421;,
  0.217714;-0.217708;-0.951422;,
  0.951420;0.217716;0.217714;,
  0.951421;-0.217711;0.217714;,
  0.951423;0.217712;-0.217707;,
  0.951424;-0.217707;-0.217707;,
  0.000000;-0.707115;-0.707099;,
  0.000000;0.707111;-0.707103;,
  0.000000;-0.707111;0.707103;,
  0.000000;0.707107;0.707107;,
  0.000000;-0.975084;-0.221837;,
  0.000000;-0.975083;0.221842;,
  0.000000;0.975081;-0.221848;,
  0.000000;0.975080;0.221853;,
  0.000000;-0.221848;-0.975081;,
  0.000000;0.221853;-0.975080;,
  0.000000;-0.221848;0.975081;,
  0.000000;0.221853;0.975080;,
  0.372520;-0.313647;0.873416;,
  0.400531;0.595555;0.696340;,
  -0.086874;-0.783900;0.614779;,
  0.302358;0.730972;-0.611768;,
  0.919565;-0.392937;-0.000000;,
  -0.482892;0.875680;0.000000;,
  0.174527;-0.686023;-0.706337;,
  -0.304613;0.835951;-0.456505;,
  0.185716;-0.068263;0.980229;,
  0.205493;0.471073;-0.857824;,
  0.070865;0.222162;-0.972431;,
  -0.954274;0.298935;0.000000;,
  -0.323363;-0.944794;0.052920;,
  -0.346413;-0.937352;0.037007;,
  -0.273124;-0.951691;0.140311;,
  0.195632;0.950129;0.242863;,
  0.104020;0.994575;-0.000075;,
  0.184188;0.955237;-0.231509;,
  0.093331;-0.995635;-0.000000;,
  0.093331;-0.995635;-0.000000;,
  0.110498;-0.133570;-0.984860;,
  -0.482892;0.875680;0.000000;,
  -0.397267;0.917703;0.000000;,
  -0.948459;0.316901;0.000000;,
  -0.948900;0.315577;0.000000;,
  -0.952317;0.305110;0.000000;,
  -0.999259;-0.038488;0.000000;,
  0.919565;-0.392937;-0.000000;,
  -0.212722;-0.067146;-0.974803;,
  -0.161377;-0.021417;-0.986660;,
  0.936249;-0.351338;-0.000000;,
  -0.013639;-0.018592;-0.999734;,
  -0.999992;-0.003989;0.000000;,
  0.142445;-0.082893;0.986326;,
  0.933876;-0.357596;-0.000000;,
  0.143897;-0.033903;-0.989012;,
  0.137594;-0.090954;-0.986304;,
  -0.031914;-0.066841;0.997253;,
  -0.998679;0.051391;0.000000;,
  0.234768;-0.047941;-0.970868;,
  -0.075900;-0.000699;0.997115;,
  -0.223635;-0.039187;0.973885;,
  0.383596;0.024444;0.923177;,
  -0.463778;-0.885951;0.000000;,
  -0.203407;-0.012962;-0.979008;,
  -0.068121;-0.004341;-0.997668;,
  -0.444603;-0.895728;-0.000002;,
  -0.444605;-0.895727;0.000000;,
  -0.884400;-0.466730;-0.000006;,
  0.198958;0.006628;0.979986;,
  -0.265356;-0.964151;0.000000;,
  0.122008;-0.003220;-0.992524;,
  0.190726;-0.005033;-0.981630;,
  -0.808944;-0.587886;0.000000;,
  -0.973307;-0.229508;0.000000;,
  -0.400255;-0.916404;0.000000;,
  -0.177862;-0.984055;0.000000;,
  -0.223195;0.022151;0.974522;,
  0.177718;0.984081;0.000000;,
  0.094572;0.006026;0.995500;,
  0.059296;-0.998240;-0.000000;,
  0.098287;0.995158;0.000000;,
  0.256082;0.015405;-0.966532;,
  0.221868;0.013225;0.974987;,
  0.376290;-0.926502;-0.000000;,
  0.094769;0.003159;0.995494;,
  0.094769;0.003159;-0.995494;,
  0.189541;0.006316;0.981852;,
  -0.013379;-0.999911;0.000000;,
  0.337036;0.941492;0.000000;,
  0.360061;0.011996;-0.932851;,
  0.436439;0.014536;0.899616;,
  0.401365;-0.915918;-0.000000;,
  0.760664;0.025331;0.648651;,
  0.868303;0.028915;-0.495191;,
  0.945422;0.031481;0.324325;,
  0.469221;-0.883081;-0.000000;,
  0.997976;0.063597;0.000000;,
  0.290124;0.017657;0.956826;,
  0.315829;-0.948816;-0.000000;,
  0.205816;0.978591;0.000000;,
  0.109283;-0.002883;-0.994006;,
  0.131093;-0.003471;0.991364;,
  -0.011379;-0.999935;0.000000;,
  0.152824;-0.004051;0.988245;,
  0.999652;-0.026384;-0.000000;,
  0.152841;-0.004072;0.988242;,
  0.066632;-0.997778;-0.000000;,
  0.155070;-0.015390;0.987784;,
  0.155075;-0.015391;-0.987783;,
  0.158408;-0.015721;0.987249;,
  0.158416;-0.015722;-0.987247;,
  0.995110;-0.098772;-0.000000;,
  0.151731;-0.015059;0.988307;,
  0.151731;-0.015059;-0.988307;,
  0.006731;-0.999809;0.018363;,
  -0.011791;0.941183;-0.337692;,
  -0.061708;-0.955519;0.288400;,
  -0.019902;0.701359;0.712530;,
  0.040296;-0.079542;0.996017;,
  -0.065262;0.539500;-0.839452;,
  -0.044004;0.998843;-0.019414;,
  -0.058180;0.998306;0.000000;,
  -0.044042;0.998841;0.019414;,
  -0.101417;-0.979375;0.174754;,
  -0.196230;-0.980558;-0.000022;,
  -0.061968;-0.998078;0.000000;,
  0.717685;-0.621459;-0.314193;,
  -0.931394;-0.343513;0.120435;,
  -0.928682;-0.334810;0.159536;,
  -0.922169;-0.313345;0.226759;,
  0.152072;-0.983126;-0.101671;,
  -0.013217;-0.059461;0.998143;,
  -0.360687;0.353503;-0.863099;,
  0.507770;-0.836708;-0.205156;,
  -0.238500;-0.965538;-0.104181;,
  0.123917;0.990958;0.051441;,
  0.078663;0.995680;0.049327;,
  -0.082501;0.358450;-0.929896;,
  -0.813060;-0.579840;-0.052137;,
  -0.940922;-0.334164;-0.054773;,
  0.169241;0.984148;0.053007;,
  -0.570603;-0.817874;-0.074128;,
  -0.253879;-0.909568;0.328985;,
  0.892342;-0.104229;0.439161;,
  0.883392;-0.004048;0.468617;,
  0.709203;0.534750;0.459427;,
  0.280706;0.920802;-0.270791;,
  0.453231;0.719263;-0.526538;,
  0.876701;0.106498;-0.469098;,
  -0.290759;-0.947903;0.130152;,
  -0.359489;-0.916775;-0.174045;,
  -0.305333;-0.946404;0.105309;,
  0.294110;0.909510;0.293753;,
  0.205994;0.978553;-0.000249;,
  -0.012657;0.999920;0.000000;,
  -0.190877;-0.378433;0.905734;,
  -0.217140;-0.094182;-0.971586;,
  0.120320;-0.814873;-0.567015;,
  -0.947941;0.318445;0.000000;,
  0.113586;-0.127308;0.985338;,
  0.118122;-0.136753;0.983537;,
  0.067903;-0.995891;0.059917;,
  -0.813581;0.581452;0.000000;,
  -0.999259;-0.038488;0.000000;,
  0.202493;-0.339102;0.918698;,
  0.166647;-0.064920;0.983877;,
  0.292339;-0.122419;0.948447;,
  -0.999992;-0.003984;-0.000004;,
  -0.044801;-0.093901;-0.994573;,
  -0.029901;-0.062671;-0.997586;,
  0.930895;-0.365286;-0.000000;,
  0.936249;-0.351338;-0.000000;,
  0.934376;-0.356289;-0.000000;,
  0.935068;-0.354469;-0.000000;,
  -0.999785;0.020714;0.000000;,
  -0.047809;-0.100133;0.993825;,
  -0.037345;-0.120815;0.991972;,
  -0.223540;-0.069857;0.972188;,
  -0.196894;-0.123833;0.972573;,
  0.263217;-0.123330;-0.956821;,
  0.935369;-0.353674;-0.000000;,
  -0.877220;-0.480088;0.000000;,
  0.256078;-0.035213;0.966015;,
  0.000000;0.000000;-1.000000;,
  0.127350;-0.026274;0.991510;,
  -0.438385;-0.898787;0.000000;,
  0.007312;-0.020466;0.999764;,
  0.000000;0.000000;1.000000;,
  -0.999723;0.023542;0.000000;,
  -0.960209;-0.279283;0.000000;,
  -0.821148;-0.570715;0.000000;,
  -0.997176;0.075104;0.000000;,
  0.235940;-0.023416;-0.971485;,
  0.308760;-0.030644;-0.950646;,
  0.130106;0.008291;0.991465;,
  0.094252;0.005736;0.995532;,
  0.094252;0.005736;-0.995532;,
  0.325544;0.945527;0.000000;,
  0.256081;0.015405;0.966532;,
  0.221868;0.013225;-0.974987;,
  0.189541;0.006316;-0.981852;,
  0.066616;0.002219;0.997776;,
  0.284532;0.958667;0.000000;,
  0.258014;0.966141;0.000000;,
  0.360061;0.011996;0.932851;,
  0.330821;-0.943693;-0.000000;,
  0.436439;0.014536;-0.899616;,
  0.438673;0.898647;0.000000;,
  0.290781;0.018530;-0.956610;,
  0.290124;0.017657;-0.956826;,
  0.170310;0.985391;0.000000;,
  0.228094;0.973639;0.000000;,
  0.205816;0.978591;0.000000;,
  0.058343;-0.001540;0.998295;,
  0.109281;-0.002883;0.994007;,
  0.066632;-0.997778;-0.000000;,
  0.131098;-0.003459;-0.991363;,
  0.152838;-0.004032;-0.988243;,
  0.183432;0.983032;0.000000;,
  -0.031318;0.003108;0.999505;,
  0.258465;0.966021;0.000000;,
  0.250907;0.968011;0.000000;,
  0.043780;-0.999041;-0.000000;,
  0.243333;0.969943;0.000000;,
  0.235745;0.971815;0.000000;,
  0.151731;-0.015060;-0.988307;,
  0.045256;-0.998975;-0.000000;,
  0.046733;-0.998907;-0.000000;,
  -0.930191;-0.344912;0.125623;,
  -0.230378;-0.973101;0.000000;,
  0.021773;0.433825;0.900734;,
  -0.926763;-0.330250;0.179015;,
  0.574045;-0.600180;-0.557007;,
  0.756867;-0.551750;-0.350321;,
  0.283180;-0.043406;0.958084;,
  0.429925;-0.199997;0.880435;,
  -0.414816;0.192903;-0.889222;,
  0.161938;-0.088675;0.982809;,
  -0.474123;0.188292;-0.860089;,
  -0.523507;0.849155;0.069827;,
  -0.565195;0.775066;-0.282536;,
  -0.350824;0.934215;0.064541;,
  -0.147089;0.988161;0.043622;,
  -0.185639;-0.976532;-0.109191;,
  -0.049424;-0.066690;0.996549;,
  -0.049863;-0.070871;0.996238;,
  -0.949968;-0.311293;-0.025647;,
  -0.045373;-0.065246;0.996837;,
  -0.045365;-0.065249;0.996837;,
  -0.072184;0.355181;-0.932006;,
  -0.067650;0.355240;-0.932324;,
  0.684518;-0.451313;-0.572496;,
  -0.171868;-0.950080;0.260402;,
  0.369905;-0.725212;-0.580722;,
  0.998028;-0.037683;-0.050203;,
  -0.198289;-0.128877;-0.971634;,
  -0.001686;-0.728423;-0.685126;,
  0.115699;-0.144220;-0.982759;,
  -0.884402;-0.466727;-0.000013;,
  -0.035068;-0.113449;-0.992925;,
  0.935369;-0.353674;-0.000000;,
  0.059296;-0.998240;-0.000000;,
  -0.013379;-0.999910;0.000000;,
  0.094572;0.006026;-0.995500;,
  0.290780;0.018530;0.956610;,
  0.258014;0.966141;0.000000;,
  0.868304;0.028913;-0.495189;,
  -0.011379;-0.999935;0.000000;,
  0.999652;-0.026384;-0.000000;,
  0.152839;-0.004034;-0.988243;,
  0.151731;-0.015060;0.988307;,
  0.235745;0.971815;0.000000;,
  0.171959;0.262233;0.949560;,
  0.169248;0.984147;0.053002;,
  -0.061263;0.352631;-0.933755;,
  1.000000;0.000000;0.000000;;
  307;
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
  3;140,50,47;,
  3;65,51,45;,
  3;138,139,48;,
  3;58,146,137;,
  3;55,143,175;,
  3;176,40,48;,
  3;177,69,68;,
  3;178,135,46;,
  3;53,172,163;,
  3;63,179,78;,
  3;60,75,76;,
  3;180,181,73;,
  3;136,141,57;,
  3;141,142,56;,
  3;56,142,143;,
  3;135,144,182;,
  3;144,145,59;,
  3;59,145,146;,
  3;183,179,63;,
  3;61,63,64;,
  3;64,65,61;,
  3;69,85,84;,
  3;65,184,66;,
  3;44,67,44;,
  3;185,186,187;,
  3;188,72,88;,
  3;189,71,190;,
  3;191,70,192;,
  3;64,72,188;,
  3;75,91,92;,
  3;193,194,74;,
  3;64,63,195;,
  3;196,197,77;,
  3;80,81,97;,
  3;198,199,81;,
  3;46,79,200;,
  3;201,193,201;,
  3;202,83,83;,
  3;66,184,202;,
  3;83,83,100;,
  3;203,187,82;,
  3;88,87,86;,
  3;71,204,204;,
  3;86,87,108;,
  3;205,73,89;,
  3;206,90,93;,
  3;91,75,121;,
  3;207,77,208;,
  3;209,195,210;,
  3;95,96,211;,
  3;79,129,131;,
  3;212,78,94;,
  3;79,213,214;,
  3;98,98,101;,
  3;215,99,216;,
  3;85,69,217;,
  3;218,98,98;,
  3;219,103,118;,
  3;216,99,103;,
  3;100,104,104;,
  3;217,102,220;,
  3;221,106,110;,
  3;222,107,105;,
  3;223,223,224;,
  3;204,71,106;,
  3;111,115,113;,
  3;224,109,109;,
  3;107,111,225;,
  3;108,112,226;,
  3;114,113,115;,
  3;112,116,116;,
  3;227,110,114;,
  3;109,109,228;,
  3;117,117,117;,
  3;104,104,119;,
  3;102,229,230;,
  3;101,231,231;,
  3;122,126,124;,
  3;232,120,233;,
  3;234,122,235;,
  3;90,206,123;,
  3;125,125,125;,
  3;123,127,236;,
  3;237,121,238;,
  3;233,120,239;,
  3;128,130,133;,
  3;80,240,130;,
  3;241,241,242;,
  3;96,95,243;,
  3;132,132,132;,
  3;244,242,245;,
  3;129,246,134;,
  3;247,243,248;,
  3;145,53,54;,
  3;52,53,145;,
  3;42,52,144;,
  3;142,62,61;,
  3;141,61,62;,
  3;136,47,61;,
  3;249,149,148;,
  3;185,137,139;,
  3;143,61,45;,
  3;54,250,137;,
  3;251,186,139;,
  3;43,49,140;,
  3;147,151,154;,
  3;252,150,149;,
  3;253,147,254;,
  3;42,255,256;,
  3;153,257,158;,
  3;255,152,258;,
  3;259,257,153;,
  3;260,261,262;,
  3;156,157,161;,
  3;263,262,157;,
  3;151,155,264;,
  3;152,265,266;,
  3;159,160,267;,
  3;155,159,162;,
  3;265,268,269;,
  3;270,158,271;,
  3;41,48,165;,
  3;55,41,166;,
  3;40,164,165;,
  3;57,167,168;,
  3;52,42,170;,
  3;56,55,173;,
  3;43,168,169;,
  3;253,169,272;,
  3;53,52,171;,
  3;54,163,273;,
  3;56,174,167;,
  3;272,274,253;,
  3;169,275,272;,
  3;140,47,136;,
  3;65,45,61;,
  3;138,48,41;,
  3;58,137,59;,
  3;55,138,41;,
  3;137,48,139;,
  3;177,68,276;,
  3;178,46,277;,
  3;53,163,54;,
  3;63,78,212;,
  3;60,76,278;,
  3;180,73,205;,
  3;136,57,43;,
  3;141,56,57;,
  3;56,143,55;,
  3;135,182,46;,
  3;144,59,182;,
  3;59,146,58;,
  3;47,63,61;,
  3;61,64,62;,
  3;64,61,62;,
  3;69,84,68;,
  3;65,66,51;,
  3;44,44,191;,
  3;185,187,203;,
  3;188,88,279;,
  3;189,190,280;,
  3;191,192,194;,
  3;64,188,65;,
  3;75,92,76;,
  3;193,74,74;,
  3;64,195,209;,
  3;196,77,207;,
  3;80,97,240;,
  3;198,81,80;,
  3;46,200,50;,
  3;201,201,281;,
  3;202,83,202;,
  3;66,202,202;,
  3;83,100,282;,
  3;203,82,215;,
  3;88,86,279;,
  3;71,204,190;,
  3;86,108,283;,
  3;205,89,222;,
  3;206,93,210;,
  3;91,121,237;,
  3;207,208,234;,
  3;209,210,93;,
  3;95,211,94;,
  3;79,131,213;,
  3;212,94,211;,
  3;79,214,200;,
  3;98,101,101;,
  3;215,216,203;,
  3;85,217,284;,
  3;218,98,218;,
  3;219,118,285;,
  3;216,103,219;,
  3;100,104,282;,
  3;217,220,284;,
  3;221,110,227;,
  3;222,105,205;,
  3;223,224,286;,
  3;204,106,221;,
  3;111,113,225;,
  3;224,109,286;,
  3;107,225,105;,
  3;108,226,283;,
  3;114,115,287;,
  3;112,116,226;,
  3;227,114,287;,
  3;109,228,228;,
  3;117,117,117;,
  3;104,119,119;,
  3;102,230,220;,
  3;101,231,101;,
  3;122,124,235;,
  3;232,233,232;,
  3;234,235,207;,
  3;90,123,288;,
  3;125,125,289;,
  3;123,236,288;,
  3;237,238,290;,
  3;233,239,239;,
  3;128,133,291;,
  3;80,130,128;,
  3;241,242,244;,
  3;96,243,247;,
  3;132,132,132;,
  3;244,245,292;,
  3;129,134,131;,
  3;247,248,248;,
  3;145,54,146;,
  3;52,145,144;,
  3;42,144,135;,
  3;142,61,143;,
  3;141,62,142;,
  3;136,61,141;,
  3;293,255,293;,
  3;185,139,186;,
  3;143,45,138;,
  3;54,137,146;,
  3;251,139,138;,
  3;43,140,136;,
  3;147,154,254;,
  3;252,149,249;,
  3;49,259,140;,
  3;274,147,253;,
  3;153,158,270;,
  3;255,258,256;,
  3;259,153,261;,
  3;260,262,263;,
  3;156,161,294;,
  3;263,157,156;,
  3;151,264,154;,
  3;152,266,258;,
  3;159,267,162;,
  3;155,162,264;,
  3;265,269,266;,
  3;270,271,295;,
  3;41,165,166;,
  3;55,166,173;,
  3;40,165,48;,
  3;57,168,43;,
  3;52,170,171;,
  3;56,173,174;,
  3;43,169,49;,
  3;53,171,172;,
  3;54,273,250;,
  3;56,167,57;,
  3;296,296,296;,
  3;296,165,164;,
  3;296,166,165;,
  3;296,296,165;,
  3;296,296,296;,
  3;296,296,296;,
  3;296,275,296;,
  3;296,275,296;,
  3;296,275,169;;
 }
 MeshTextureCoords {
  179;
  0.280980;0.558750;,
  0.295880;0.568350;,
  0.285140;0.582760;,
  0.270580;0.575140;,
  0.229990;0.523970;,
  0.250220;0.531250;,
  0.243870;0.549230;,
  0.224860;0.554320;,
  0.239600;0.579350;,
  0.255610;0.562350;,
  0.224030;0.596450;,
  0.209760;0.558380;,
  0.284910;0.505140;,
  0.270350;0.497510;,
  0.266410;0.474800;,
  0.289800;0.480220;,
  0.210140;0.516370;,
  0.308600;0.588550;,
  0.289090;0.605470;,
  0.311630;0.531050;,
  0.299880;0.517920;,
  0.315890;0.500930;,
  0.330630;0.525950;,
  0.292110;0.622890;,
  0.260940;0.622890;,
  0.265690;0.600060;,
  0.263320;0.544010;,
  0.292180;0.536270;,
  0.274520;0.521520;,
  0.259610;0.511930;,
  0.325500;0.556310;,
  0.305280;0.549030;,
  0.263390;0.457390;,
  0.246890;0.491730;,
  0.234990;0.473370;,
  0.345360;0.563910;,
  0.320500;0.606910;,
  0.345740;0.521900;,
  0.294560;0.457390;,
  0.331460;0.483820;,
  0.241720;0.554100;,
  0.241440;0.554700;,
  0.241250;0.554570;,
  0.241130;0.555250;,
  0.241050;0.555380;,
  0.240890;0.555280;,
  0.240360;0.555540;,
  0.240820;0.555920;,
  0.239480;0.559500;,
  0.241270;0.555490;,
  0.241640;0.556250;,
  0.241070;0.556510;,
  0.237090;0.555810;,
  0.240300;0.555030;,
  0.241140;0.560340;,
  0.241120;0.555400;,
  0.241110;0.555380;,
  0.242190;0.554850;,
  0.241620;0.554630;,
  0.244780;0.557770;,
  0.252880;0.563280;,
  0.249390;0.568390;,
  0.241350;0.554860;,
  0.241480;0.554760;,
  0.241600;0.554940;,
  0.241400;0.555000;,
  0.241380;0.554970;,
  0.241210;0.555240;,
  0.241000;0.553850;,
  0.240640;0.554120;,
  0.238410;0.550680;,
  0.240410;0.554550;,
  0.237070;0.553020;,
  0.242260;0.555150;,
  0.242050;0.555790;,
  0.241460;0.555240;,
  0.241100;0.554690;,
  0.241230;0.555050;,
  0.240910;0.555080;,
  0.241110;0.555400;,
  0.241110;0.555400;,
  0.241090;0.555390;,
  0.241060;0.555570;,
  0.241100;0.555410;,
  0.241240;0.555200;,
  0.241240;0.555230;,
  0.241260;0.555240;,
  0.241240;0.555250;,
  0.241380;0.555010;,
  0.241380;0.555000;,
  0.241380;0.555020;,
  0.241350;0.555020;,
  0.241500;0.554780;,
  0.241470;0.554790;,
  0.241490;0.554780;,
  0.241500;0.554760;,
  0.241110;0.555400;,
  0.241110;0.555400;,
  0.241100;0.555400;,
  0.241250;0.555240;,
  0.241250;0.555240;,
  0.241250;0.555240;,
  0.241240;0.555240;,
  0.241380;0.555010;,
  0.241370;0.555010;,
  0.241380;0.555010;,
  0.241500;0.554780;,
  0.241500;0.554780;,
  0.241500;0.554780;,
  0.241500;0.554780;,
  0.241500;0.554770;,
  0.241110;0.555400;,
  0.241110;0.555400;,
  0.241110;0.555400;,
  0.241110;0.555400;,
  0.241110;0.555400;,
  0.241110;0.555400;,
  0.241110;0.555400;,
  0.241110;0.555400;,
  0.241250;0.555240;,
  0.241250;0.555240;,
  0.241250;0.555240;,
  0.241250;0.555240;,
  0.241250;0.555240;,
  0.241250;0.555240;,
  0.241250;0.555240;,
  0.241250;0.555240;,
  0.241250;0.555240;,
  0.241250;0.555240;,
  0.241250;0.555240;,
  0.241110;0.555400;,
  0.241110;0.555400;,
  0.241110;0.555400;,
  0.241380;0.555010;,
  0.241380;0.555010;,
  0.241380;0.555010;,
  0.241380;0.555010;,
  0.241380;0.555010;,
  0.241380;0.555010;,
  0.241380;0.555010;,
  0.241500;0.554770;,
  0.241500;0.554780;,
  0.241500;0.554780;,
  0.241500;0.554780;,
  0.241500;0.554780;,
  0.241500;0.554780;,
  0.243190;0.559650;,
  0.245180;0.555130;,
  0.244820;0.553660;,
  0.240980;0.554880;,
  0.242520;0.554170;,
  0.242680;0.554310;,
  0.240660;0.549230;,
  0.243240;0.549830;,
  0.242800;0.554120;,
  0.242620;0.554120;,
  0.242610;0.554120;,
  0.242570;0.554000;,
  0.242610;0.554120;,
  0.242610;0.554120;,
  0.242610;0.554130;,
  0.242610;0.554120;,
  0.242610;0.554120;,
  0.242610;0.554120;,
  0.242610;0.554120;,
  0.242610;0.554120;,
  0.242610;0.554120;,
  0.242610;0.554120;,
  0.238170;0.558250;,
  0.236400;0.569220;,
  0.230060;0.562760;,
  0.244820;0.570900;,
  0.232030;0.544310;,
  0.236740;0.539760;,
  0.253350;0.548760;,
  0.228620;0.556750;,
  0.246700;0.539550;,
  0.255170;0.557220;,
  0.229320;0.550010;;
 }
}
