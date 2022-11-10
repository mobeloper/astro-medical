
#ifdef ___EXPORTS
#define _EXPORT __declspec(dllexport)
#else
#define _EXPORT __declspec(dllimport)
#endif

#ifndef PIXELMAX
#define PIXELMAX 65535
#endif

_EXPORT void SetNumThread(int n);
_EXPORT int GetNumThread();

_EXPORT float FindMax(float *src,int n);
_EXPORT float FindMin(float *src,int n);
_EXPORT void FindMinMax(float *src,int n,float *minimum,float *maximum);
_EXPORT void FloatToU16(unsigned short *dst,float *src,int n,float min,float max);
_EXPORT void U16ToFloat(float *dst,unsigned short *src,int n);
_EXPORT void S16ToFloat(float *dst,short *src,int n);

_EXPORT void Add(float *dst,float *src1,float *src2,int n);
_EXPORT void Add(float *dst,float *src,float val,int n);
_EXPORT void Sub(float *dst,float *src1,float *src2,int n);
_EXPORT void Sub(float *dst,float *src,float val,int n);
_EXPORT void Diff(float *dst,float *src1,float *src2,int n);
_EXPORT void Diff(float *dst,float *src,float val,int n);
_EXPORT void Mul(float *dst,float *src1,float *src2,int n);
_EXPORT void Mul(float *dst,float *src,float val,int n);
_EXPORT void Div(float *dst,float *src1,float *src2,int n);
_EXPORT void Div(float *dst,float *src,float val,int n);
_EXPORT void Inverse(float *dst,float *src,int n);
_EXPORT void Log(float *dst,float *src,int n);
_EXPORT void Exp(float *dst,float *src,int n);
_EXPORT void NegExp(float *dst,float *src,int n);
_EXPORT void Abs(float *dst,float *src,int n);

_EXPORT void CopyBorder(float *d,float *s,int nx,int ny,int n=1);
_EXPORT void CopyInnerBorder(float *d,int width,int height);
_EXPORT void FillBorder(float *d,int nx,int ny,float val,int n=1);

_EXPORT void Median3x3(float *d, float *s, int nx, int ny);
_EXPORT void Average3x3(float *d, float *s, int nx, int ny);
_EXPORT void Average5x5(float *d, float *s, int nx, int ny);
_EXPORT void Average7x7(float *d, float *s, int nx, int ny);
_EXPORT void Average9x9(float *d, float *s, int nx, int ny);
_EXPORT void AverageNxN(float *result, float *data, int width, int height, int n);		//Smooth Function

_EXPORT void GradMag(float *d, float *s, int nx, int ny);
_EXPORT void GeomDiff(float *result, float *origIm, int width, int height, int iter, float dt=0.25, int Kmethod=0);		//Denoising Function

_EXPORT void ImInvertPixels(float *d, float *s, float nMinPixVal, float nMaxPixVal, int width, int height);	// Invert Pixel Values

_EXPORT void ImSharp9x9(float *d, float *s, int nx, int ny, int k);
_EXPORT void ImSharp(float *d, float *s, int nx, int ny, int k=5, int wz=11);				//Sharp Enhancement Function

_EXPORT void ImEdges(float *d, float *s, int width, int height, int k);	//Edge Enhancement Function
