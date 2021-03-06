// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMECAST_PUBLIC_MEDIA_EXTERNAL_AUDIO_PIPELINE_SHLIB_H_
#define CHROMECAST_PUBLIC_MEDIA_EXTERNAL_AUDIO_PIPELINE_SHLIB_H_

#include <memory>

#include "cast_media_shlib.h"
#include "chromecast_export.h"
#include "media_pipeline_backend.h"
#include "mixer_output_stream.h"

namespace chromecast {
namespace media {

class CHROMECAST_EXPORT ExternalAudioPipelineShlib {
 public:
  // Represents a media input source of the external media pipeline.
  enum class MediaInputSource {
    UNKNOWN_INPUT_SOURCE,
    HDMI,
    AIRPLAY,
  };

  // Represents a media playback state of the external media pipeline.
  enum class MediaPlaybackState {
    UNKNOWN_STATE,
    STOPPED,
    PAUSED,
    PLAYING,
  };

  // Observer for reporting requests for media volume change/muting from the
  // external media pipeline. The external pipeline should communicate the media
  // volume change requests through this observer and otherwise shouldn't change
  // the media volume in any way. Cast pipeline will call
  // SetExternalMediaVolume/SetExternalMediaMuted to actually change the volume
  // of the external media. The external pipeline must only apply the received
  // volume to the external (non-Cast) audio. It specifically must not be
  // applied to the mixer output stream. The received volume should also not be
  // reported again through this observer.
  class ExternalMediaVolumeChangeRequestObserver {
   public:
    // Called by the external pipeline to request a media volume change by the
    // cast pipeline.
    virtual void OnVolumeChangeRequest(float new_volume) = 0;

    // Called by the external pipeline to request muting/unmuting media volume
    // by the cast pipeline.
    virtual void OnMuteChangeRequest(bool new_muted) = 0;

   protected:
    virtual ~ExternalMediaVolumeChangeRequestObserver() = default;
  };

  // Media metadata which can be acquired from the external pipeline.
  struct ExternalMediaMetadata {
    ExternalMediaMetadata();
    ExternalMediaMetadata(const ExternalMediaMetadata& other);
    ~ExternalMediaMetadata();

    std::string title;
    std::string artist;
    std::string album;
    std::string genre;
    int track_num = -1;
    MediaInputSource source = MediaInputSource::HDMI;
    MediaPlaybackState state = MediaPlaybackState::UNKNOWN_STATE;
  };

  // Observer for reporting media metadata change from the external media
  // pipeline, e.g., title, artist, input source, play back state, and etc.
  class ExternalMediaMetadataChangeObserver {
   public:
    // Called when media metadata is updated.
    virtual void OnExternalMediaMetadataChanged(
        const ExternalMediaMetadata& metadata) = 0;

   protected:
    virtual ~ExternalMediaMetadataChangeObserver() = default;
  };

  // Returns whether this shlib is supported. If this returns true, it indicates
  // that the platform uses an external audio pipeline that needs to be combined
  // with Cast's media pipeline.
  static bool IsSupported();

  // Adds an external media volume observer.
  static void AddExternalMediaVolumeChangeRequestObserver(
      ExternalMediaVolumeChangeRequestObserver* observer);

  // Removes an external media volume observer. After this is called, the
  // implementation must not call any more methods on the observer.
  static void RemoveExternalMediaVolumeChangeRequestObserver(
      ExternalMediaVolumeChangeRequestObserver* observer);

  // Sets the effective volume that the external pipeline must apply. The volume
  // must be applied only to external (non-Cast) audio. Cast audio must never
  // be affected. The pipeline should not report it back through the volume
  // observer. The volume |level| is in the range [0.0, 1.0].
  static void SetExternalMediaVolume(float level);

  // Sets the effective muted state that the external pipeline must apply. The
  // mute state must be applied only to external (non-Cast) audio. Cast audio
  // must never be affected. The pipeline should not report this state back
  // through the volume observer.
  static void SetExternalMediaMuted(bool muted);

  // Adds a loopback audio observer. An observer will not be added more than
  // once without being removed first.
  static void AddExternalLoopbackAudioObserver(
      CastMediaShlib::LoopbackAudioObserver* observer);

  // Removes a loopback audio observer. An observer will not be removed unless
  // it was previously added, and will not be removed more than once without
  // being added again first.
  // Once the observer is fully removed (ie. once it is certain that
  // OnLoopbackAudio() will not be called again for the observer), the
  // observer's OnRemoved() method must be called. The OnRemoved() method must
  // be called once for each time that RemoveLoopbackAudioObserver() is called
  // for a given observer, even if the observer was not added. The
  // implementation may call OnRemoved() from any thread.
  // This function is optional to implement.
  static void RemoveExternalLoopbackAudioObserver(
      CastMediaShlib::LoopbackAudioObserver* observer);

  // Adds an external media metadata observer.
  static void AddExternalMediaMetadataChangeObserver(
      ExternalMediaMetadataChangeObserver* observer) __attribute__((__weak__));

  // Removes an external media volume observer. After this is called, the
  // implementation must not call any more methods on the observer.
  static void RemoveExternalMediaMetadataChangeObserver(
      ExternalMediaMetadataChangeObserver* observer) __attribute__((__weak__));

  // Returns an instance of MixerOutputStream from the shared library.
  // Caller will take ownership of the returned pointer.
  static std::unique_ptr<MixerOutputStream> CreateMixerOutputStream();
};

inline ExternalAudioPipelineShlib::ExternalMediaMetadata::
    ExternalMediaMetadata() = default;

inline ExternalAudioPipelineShlib::ExternalMediaMetadata::ExternalMediaMetadata(
    const ExternalMediaMetadata& other) = default;

inline ExternalAudioPipelineShlib::ExternalMediaMetadata::
    ~ExternalMediaMetadata() = default;

}  // namespace media
}  // namespace chromecast

#endif  // CHROMECAST_PUBLIC_MEDIA_EXTERNAL_AUDIO_PIPELINE_SHLIB_H_
