/* GStreamer
 * Copyright (C) 2012 Fluendo S.A. <support@fluendo.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef __OPENSLESRINGBUFFER_H__
#define __OPENSLESRINGBUFFER_H__

#include <gst/gst.h>
#include <gst/audio/gstringbuffer.h>

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

G_BEGIN_DECLS

#define GST_TYPE_OPENSLES_RING_BUFFER \
  (gst_opensles_ringbuffer_get_type())
#define GST_OPENSLES_RING_BUFFER(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_OPENSLES_RING_BUFFER,GstOpenSLESRingBuffer))
#define GST_OPENSLES_RING_BUFFER_CAST(obj) \
  ((GstOpenSLESRingBuffer*) obj)
#define GST_OPENSLES_RING_BUFFER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_OPENSLES_RING_BUFFER,GstOpenSLESRingBufferClass))
#define GST_OPENSLES_RING_BUFFER_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS((obj),GST_TYPE_OPENSLES_RING_BUFFER,GstOpenSLESRingBufferClass))
#define GST_IS_OPENSLES_RING_BUFFER(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_OPENSLES_RING_BUFFER))
#define GST_IS_OPENSLES_RING_BUFFER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_OPENSLES_RING_BUFFER))

typedef enum
{
  RB_MODE_NONE = 0,
  RB_MODE_SRC,
  RB_MODE_SINK_PCM,
  RB_MODE_SINK_COMPRESSED,
  RB_MODE_LAST
} RingBufferMode;

typedef gboolean (*AcquireFunc) (GstRingBuffer * rb, guint nbuffers,
    SLDataFormat_PCM * format);

typedef gboolean (*StateFunc) (GstRingBuffer * rb);

typedef struct _GstOpenSLESRingBuffer GstOpenSLESRingBuffer;
typedef struct _GstOpenSLESRingBufferClass GstOpenSLESRingBufferClass;

struct _GstOpenSLESRingBuffer
{
  GstRingBuffer object;

  RingBufferMode mode;

  /* engine interfaces */
  SLObjectItf engineObject;
  SLEngineItf engineEngine;

  /* outputMixObject */
  SLObjectItf outputMixObject;

  /* player interfaces */
  SLObjectItf playerObject;
  SLPlayItf playerPlay;
  SLVolumeItf playerVolume;

  /* recorder interfaces */
  SLObjectItf recorderObject;
  SLRecordItf recorderRecord;

  /* buffer queue */
  SLAndroidSimpleBufferQueueItf bufferQueue;
  gint last_readseg;

  /* vmethods */
  AcquireFunc acquire;
  StateFunc start;
  StateFunc pause;
  StateFunc stop;
};

struct _GstOpenSLESRingBufferClass
{
  GstRingBufferClass parent_class;
};

GType gst_opensles_ringbuffer_get_type (void);
GstRingBuffer *gst_opensles_ringbuffer_new (RingBufferMode mode);

G_END_DECLS
#endif /* __OPENSLESRINGBUFFER_H__ */
