using System.Runtime.InteropServices;

namespace ManagedBass.Substream
{
    public static class BassSubstream
    {
        const string DllName = "bass_substream";

        const bool DEFAULT_FREE_PARENT = false;

        public static bool FreeParent
        {
            get
            {
                var freeParent = default(int);
                if (!GetConfig(BassSubstreamAttribute.FreeParent, out freeParent))
                {
                    return DEFAULT_FREE_PARENT;
                }
                return freeParent != 0;
            }
            set
            {
                SetConfig(BassSubstreamAttribute.FreeParent, value ? 1 : 0);
            }
        }

        [DllImport(DllName)]
        static extern bool BASS_SUBSTREAM_Init();

        public static bool Init()
        {
            return BASS_SUBSTREAM_Init();
        }

        [DllImport(DllName)]
        static extern bool BASS_SUBSTREAM_SetConfig(BassSubstreamAttribute Attrib, int Value);

        public static bool SetConfig(BassSubstreamAttribute Attrib, int Value)
        {
            return BASS_SUBSTREAM_SetConfig(Attrib, Value);
        }

        [DllImport(DllName)]
        static extern bool BASS_SUBSTREAM_GetConfig(BassSubstreamAttribute Attrib, out int Value);

        public static bool GetConfig(BassSubstreamAttribute Attrib, out int Value)
        {
            return BASS_SUBSTREAM_GetConfig(Attrib, out Value);
        }

        [DllImport(DllName)]
        static extern int BASS_SUBSTREAM_StreamCreate(int Handle, long Offset, long Length, BassFlags Flags);

        public static int CreateStream(int Handle, long Offset, long Length, BassFlags Flags = BassFlags.Default)
        {
            return BASS_SUBSTREAM_StreamCreate(Handle, Offset, Length, Flags);
        }
    }

    public enum BassSubstreamAttribute
    {
        None = 0,
        FreeParent = 1
    }
}
