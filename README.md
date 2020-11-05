# BASS_SUBSTREAM

A BASS plugin which creates a stream from a parent with offset and length positions.
It was developed to support .cue files. 
The child stream will return the same attributes as the parent and simplifies other code as you don't need to track the start and end positions, BASS_ChannelGetLength functions as expected. 

Currently only one child stream can be created per master as the position is not maintained.

A .cue sheet parser can be found [here](https://github.com/aidan-g/FoxTunes/tree/master/FoxTunes.Output.Bass.Cue/Utility).  
Example integration can be found [here](https://github.com/aidan-g/FoxTunes/blob/master/FoxTunes.Output.Bass.Cue/BassCueStreamAdvise.cs).

A simple example;

```c#
// ****
// Play from position 01:00 to 02:00.
// ****

if (!Bass.Init(Bass.DefaultDevice))
{
	Assert.Fail(string.Format("Failed to initialize BASS: {0}", Enum.GetName(typeof(Errors), Bass.LastError)));
}

var sourceChannel = Bass.CreateStream("Test.mp3", 0, 0, BassFlags.Decode);
if (sourceChannel == 0)
{
	Assert.Fail(string.Format("Failed to create source stream: {0}", Enum.GetName(typeof(Errors), Bass.LastError)));
}

var playbackChannel = BassSubstream.CreateStream(sourceChannel, GetPosition(sourceChannel, "01:00"), GetPosition(sourceChannel, "01:00"));
if (playbackChannel == 0)
{
	Assert.Fail(string.Format("Failed to create playback stream: {0}", Enum.GetName(typeof(Errors), Bass.LastError)));
}

if (!Bass.ChannelPlay(playbackChannel))
{
	Assert.Fail(string.Format("Failed to start playback stream: {0}", Enum.GetName(typeof(Errors), Bass.LastError)));
}


......


private static long GetPosition(int handle, string position)
{
	if (string.IsNullOrEmpty(position))
	{
		return 0;
	}
	var offset = TimeSpan.Parse(position).TotalSeconds;
	return Bass.ChannelSeconds2Bytes(handle, offset);
}
````
