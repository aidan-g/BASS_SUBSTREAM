using NUnit.Framework;
using System;
using System.Diagnostics;
using System.IO;
using System.Threading;

namespace ManagedBass.Substream.Tests
{
    [TestFixture]
    public class Tests
    {
        private static readonly string Location = Path.GetDirectoryName(typeof(Tests).Assembly.Location);

        [TestCase("01 Botanical Dimensions.m4a", null, "00:01:00")]
        [TestCase("01 Botanical Dimensions.m4a", "00:01:00", "00:01:00")]
        [TestCase("01 Botanical Dimensions.m4a", "00:02:00", "00:01:00")]
        [TestCase("01 Botanical Dimensions.m4a", "00:03:00", null)]
        public void Test001(string name, string offset, string length)
        {
            if (!Bass.Init(Bass.DefaultDevice))
            {
                Assert.Fail(string.Format("Failed to initialize BASS: {0}", Enum.GetName(typeof(Errors), Bass.LastError)));
            }

            var fileName = Path.Combine(Location, "Media", name);
            var sourceChannel = Bass.CreateStream(fileName, 0, 0, BassFlags.Decode);
            if (sourceChannel == 0)
            {
                Assert.Fail(string.Format("Failed to create source stream: {0}", Enum.GetName(typeof(Errors), Bass.LastError)));
            }

            var playbackChannel = BassSubstream.CreateStream(sourceChannel, GetPosition(sourceChannel, offset), GetPosition(sourceChannel, length));
            if (playbackChannel == 0)
            {
                Assert.Fail(string.Format("Failed to create playback stream: {0}", Enum.GetName(typeof(Errors), Bass.LastError)));
            }

            var channelLength = Bass.ChannelGetLength(playbackChannel);
            var channelLengthSeconds = Bass.ChannelBytes2Seconds(playbackChannel, channelLength);

            do
            {
                if (Bass.ChannelIsActive(playbackChannel) == PlaybackState.Stopped)
                {
                    break;
                }

                var channelPosition = Bass.ChannelGetPosition(playbackChannel);
                var channelPositionSeconds = Bass.ChannelBytes2Seconds(playbackChannel, channelPosition);

                Debug.WriteLine(
                    "{0}/{1}",
                    TimeSpan.FromSeconds(channelPositionSeconds).ToString("g"),
                    TimeSpan.FromSeconds(channelLengthSeconds).ToString("g")
                );

                Thread.Sleep(1000);
            } while (true);

            if (!Bass.StreamFree(sourceChannel))
            {
                Assert.Fail(string.Format("Failed to free the source stream: {0}", Enum.GetName(typeof(Errors), Bass.LastError)));
            }

            if (!Bass.StreamFree(playbackChannel))
            {
                Assert.Fail(string.Format("Failed to free the playback stream: {0}", Enum.GetName(typeof(Errors), Bass.LastError)));
            }

            if (!Bass.Free())
            {
                Assert.Fail(string.Format("Failed to free BASS: {0}", Enum.GetName(typeof(Errors), Bass.LastError)));
            }
        }

        private static long GetPosition(int handle, string position)
        {
            var offset = TimeSpan.Parse(position).TotalMilliseconds;
            return Bass.ChannelSeconds2Bytes(handle, offset);
        }
    }
}
