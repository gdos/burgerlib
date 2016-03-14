#if 0
//
// Generated by 2.0.21256.0
//
//   fxc ..\source\graphics\shadersxbox360\vs20sprite2d360.x360sl /nologo
//    /FhD:\projects\burger\libs\burgerlib\source\graphics\shadersxbox360\vs20sprite2d360.h
//    /XFdD:\projects\burger\libs\burgerlib\projects\temp\burgerlibv10x36dbg\vs20sprite2d360.updb
//    /Vng_vs20sprite2d360 /Tvs_2_0
//
//
// Parameters:
//
//   float4 XYWidthHeight;
//
//
// Registers:
//
//   Name          Reg   Size
//   ------------- ----- ----
//   XYWidthHeight c0       1
//

// Shader type: vertex 

xvs_3_0
defconst XYWidthHeight, float, vector, [1, 4], c0
config AutoSerialize=false
config AutoResource=false
config VsMaxReg=1
config VsResource=1

dcl_index r0.x
dcl_texcoord o0.xy

def c252, 0, 0, 0, 0
def c253, 0, 0, 0, 0
def c254, 0, 0, 0, 0
def c255, -2, 2, -1, 1


    exec
    vfetch r1.xy__, r0.x, position
    vfetch r0.xy__, r0.x, texcoord
    alloc position
    exec
    mad r0.z, c0.x, c255.y, c255.z
    mad r0.w, c0.y, c255.x, c255.w
    mad oPos.xy01, r1.xy, c0.zw, r0.zw
    alloc interpolators
    exece
    mov o0.xy, r0.xy

// PDB hint 00000000-00000000-00000000

#endif

// This microcode is in native DWORD byte order.

const DWORD g_vs20sprite2d360[] =
{
    0x102a1101, 0x000000ec, 0x000000b8, 0x00000000, 0x00000024, 0x0000008c, 
    0x000000b4, 0x00000000, 0x00000000, 0x00000064, 0x0000001c, 0x00000057, 
    0xfffe0300, 0x00000001, 0x0000001c, 0x00000000, 0x00000050, 0x00000030, 
    0x00020000, 0x00010000, 0x00000040, 0x00000000, 0x58595769, 0x64746848, 
    0x65696768, 0x7400abab, 0x00010003, 0x00010004, 0x00010000, 0x00000000, 
    0x76735f33, 0x5f300032, 0x2e302e32, 0x31323536, 0x2e3000ab, 0x00000000, 
    0x00000001, 0x00000000, 0x00000000, 0x00000014, 0x00fc0010, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000040, 0x00000078, 0x00010001, 
    0x00000000, 0x00000000, 0x00000821, 0x00000001, 0x00000002, 0x00000001, 
    0x00000290, 0x00100003, 0x00305004, 0x00003050, 0x00001008, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xc0000000, 
    0x40000000, 0xbf800000, 0x3f800000, 0x30052003, 0x00001200, 0xc2000000, 
    0x00003005, 0x00001200, 0xc4000000, 0x00001008, 0x00002200, 0x00000000, 
    0x05f81000, 0x00000fc8, 0x00000000, 0x05f80000, 0x00000fc8, 0x00000000, 
    0xc8040000, 0x006cb1c6, 0x0b00ffff, 0xc8080000, 0x00b16c1b, 0x0b00ffff, 
    0xc88bc03e, 0x00b01a1a, 0xab010000, 0xc8038000, 0x00b0b000, 0xe2000000, 
    0x00000000, 0x00000000, 0x00000000
};
