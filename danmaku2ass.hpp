#if defined(XYZLIBRARY_EXPORT)
#   define libdanmaku2ass   __declspec(dllexport)
#else
#   define libdanmaku2ass   __declspec(dllimport)
#endif  