#pragma once
//”z—ñ‚Ìíœ
#define SAFE_DELETE_ARRAY(p) if(p != nullptr){ delete[] p; p = nullptr;}
//íœ
#define SAFE_DELETE(p) if(p != nullptr){delete p; p = nullptr;}
//ƒƒ‚ƒŠ‚ÌŠJ•ú
#define SAFE_RELEASE(p) if(p != nullptr){ p->Release(); p = nullptr;}