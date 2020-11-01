using System.Runtime.InteropServices;

namespace ManagedBass.Substream
{
    public static class BassSubstream
    {
        const string DllName = "bass_substream";

        [DllImport(DllName)]
        static extern int BASS_SUBSTREAM_StreamCreate(int Handle, long Offset, long Length, BassFlags Flags);

        public static int CreateStream(int Handle, long Offset, long Length, BassFlags Flags = BassFlags.Default)
        {
            return BASS_SUBSTREAM_StreamCreate(Handle, Offset, Length, Flags);
        }
    }
}
