using System;
using System.Runtime.InteropServices;

namespace LoLUpdater
{
    internal class NativeMethods
    {
        public static const string s_kernel = "kernel32.dll";
        
        [DllImport(s_kernel, CharSet = CharSet.Unicode)]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern void DeleteFile(string file);

        [DllImport(s_kernel, CharSet = CharSet.Ansi, BestFitMapping = false]
        public static extern IntPtr GetProcAddress(IntPtr hModule, string proc);

        // http://msdn.microsoft.com/en-us/library/windows/desktop/ms724482%28v=vs.85%29.aspx
        [DllImport(s_kernel, CharSet = CharSet.Unicode)]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool IsProcessorFeaturePresent(uint feature);

        [DllImport(s_kernel, CharSet = CharSet.Unicode)]
        public static extern IntPtr LoadLibrary(string file);
        
    }
}
