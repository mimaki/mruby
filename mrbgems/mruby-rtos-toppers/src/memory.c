#include <stdio.h>
#include <string.h>
#include "mruby.h"
#include "mruby/variable.h"
#include "mruby/class.h"
#include "mruby/hash.h"
#include "mruby/string.h"
#include "mruby/data.h"
#include "rtosif.h"

/* MemoryBuffer information */
typedef struct mrb_mbf_t {
  ID id;          /* MemoryPool ID */
  size_t blksz;   /* Size of memory block */
  void *buf;      /* Pointer to memory block */
} mrb_mbf_t;

static void
mrb_mpl_free(mrb_state *mrb, void *data)
{
  if (data) {
    T_CMPF *pcmpf = (T_CMPF*)data;
    if (pcmpf->mpf) mrb_free(mrb, pcmpf->mpf);
    mrb_free(mrb, data);
  }
}
const static struct mrb_data_type mrb_mpl_type = {"MemoryPool", mrb_mpl_free};

/*
 *  call-seq:
 *     MemoryPool.new(id=nil, blksz=256, blkcnt=16)  # => MemoryPool
 *
 *  Create fixed size memory pool.
 *  If +id+ is specified, the memory pool is associated with ID.
 *
 *  Parameters:
 *    +id+      ID of 'existing' memory pool
 *      !=nil     Create new MemoryPool
 *    +blksz+   Size of memory pool block (fixed size)
 *    +blkcnt+  Number of memory pool block
 *
 *  Returns MemoryPool object.
 */
static mrb_value
mrb_mpl_init(mrb_state *mrb, mrb_value self)
{
  ID id;
  mrb_value ido = mrb_nil_value();
  mrb_int blksz = 256;
  mrb_int blkcnt = 16;
  T_CMPF *pcmpf;

  DATA_TYPE(self) = &mrb_mpl_type;
  pcmpf = (T_CMPF*)mrb_malloc(mrb, sizeof(T_CMPF));
  DATA_PTR(self) = pcmpf;
  memset(pcmpf, 0, sizeof(T_CMPF));

  mrb_get_args(mrb, "|oii", &ido, &blksz, &blkcnt);

  if (mrb_nil_p(ido)) {
    pcmpf->mpfatr = TA_TFIFO;
    pcmpf->blkcnt = blkcnt;
    pcmpf->blksz  = blksz;
    pcmpf->mpf    = mrb_malloc(mrb, blksz * blkcnt);
    id = RTOS_acre_mpf(mrb, pcmpf);
  }
  else {
    id = mrb_fixnum(ido);
  }
// printf("MemoryPool.new: id=%d, blksz=%d, blkcnt=%d, buf=%p\n", id, blksz, blkcnt, pcmpf->mpf);

  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@id"), mrb_fixnum_value(id));
  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@blksz"), mrb_fixnum_value(blksz));

  return self;
}

/*
 *  call-seq:
 *     MemoryPool.open(id, blksz)  # => MemoryPool
 *
 *  Open fixed size memory pool.
 *
 *  Parameters:
 *    +id+      ID of 'existing' memory pool
 *    +blksz+   Size of memory pool block (fixed size)
 *
 *  Returns MemoryPool object.
 */
mrb_value
mrb_mpl_open(mrb_state *mrb, mrb_value self)
{
  struct RClass *rtos = mrb_module_get(mrb, "RTOS");
  struct RClass *mpl = mrb_class_get_under(mrb, rtos, "MemoryPool");
  mrb_int id, blksz;
  mrb_value args[2];

  mrb_get_args(mrb, "ii", &id, &blksz);
  args[0] = mrb_fixnum_value(id);
  args[1] = mrb_fixnum_value(blksz);

  return mrb_obj_new(mrb, mpl, 2, args);
}


static void
mrb_mbf_free(mrb_state *mrb, void *data)
{
  if (data) {
    mrb_mbf_t *mbf = (mrb_mbf_t*)data;
    if (mbf->buf) {
      RTOS_rel_mpf(mrb, mbf->id, mbf->buf);
    }
    mrb_free(mrb, mbf);
  }
}
const static struct mrb_data_type mrb_mbf_type = {"MemoryBuffer", mrb_mbf_free};

/*
 *  call-seq:
 *     MemoryBuffer.new(id, blksz, tmo=:forever)  # => MemoryBuffer
 *
 *  Get MemoryBuffer from MemoryPool.
 *
 *  Parameters:
 *    +id+      MemoryPool ID
 *    +blksz+   Block size of memory buffer
 *    +tmo+     Wait time for getting buffer
 *      :polling  No wait
 *      :forever  Wait forever
 *      other     Wait time (ms)
 *
 *  Returns MemoryBuffer object.
 */
mrb_value
mrb_mbf_init(mrb_state *mrb, mrb_value self)
{
  mrb_int id;
  mrb_int blksz;
  mrb_value tmo = mrb_symbol_value(mrb_intern_lit(mrb, "forever"));
  mrb_mbf_t *data;

  mrb_get_args(mrb, "ii|o", &id, &blksz, &tmo);

  DATA_TYPE(self) = &mrb_mbf_type;
  DATA_PTR(self) = data = (mrb_mbf_t*)mrb_malloc(mrb, sizeof(mrb_mbf_t));

  data->id = id;
  data->blksz = blksz;

  if (mrb_fixnum_p(tmo)) {
    RTOS_tget_mpf(mrb, id, &data->buf, mrb_fixnum(tmo));
  }
  else if (mrb_symbol_p(tmo)) {
    if (mrb_symbol(tmo) == mrb_intern_lit(mrb, "polling")) {
      RTOS_pget_mpf(mrb, id, &data->buf);  /* Polling */
    }
    else if (mrb_symbol(tmo) == mrb_intern_lit(mrb, "forever")) {
      RTOS_get_mpf(mrb, id, &data->buf);   /* Wati forever */
    }
    else {
      mrb_raisef(mrb, E_ARGUMENT_ERROR, "invalid timeout value :%S", mrb_sym2str(mrb, mrb_symbol(tmo)));
    }
  }
  else {
    mrb_raisef(mrb, E_ARGUMENT_ERROR, "invalid timeout value %S", mrb_funcall(mrb, tmo, "to_s", 0));
  }

  return self;
}

/*
 *  call-seq:
 *     mb.release  # => nil
 *
 *  Release MemoryBuffer to MemoryPool.
 *
 *  Parameters:
 *
 *  Returns nil.
 */
mrb_value
mrb_mbf_rel(mrb_state *mrb, mrb_value self)
{
  mrb_mbf_t *mbf = DATA_PTR(self);
  if (mbf) {
    if (mbf->buf) {
      RTOS_rel_mpf(mrb, mbf->id, mbf->buf);
    }
    mbf->buf = 0;   /* released */
    mbf->blksz = 0;
    /* mrb_mbf_t does not free here */
  }

  return mrb_nil_value();
}

/*
 *  call-seq:
 *     mb.data = dt  # => String
 *
 *  Set data to MemoryBuffer.
 *
 *  Parameters:
 *    +dt+    Data to be set in the MemoryBuffer
 *
 *  Returns set data.
 */
mrb_value
mrb_mbf_dset(mrb_state *mrb, mrb_value self)
{
  mrb_value src;
  mrb_mbf_t *data = DATA_PTR(self);
  size_t len;

  if (!data || !data->buf) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "MessageBuffer was already released.");
  }

  mrb_get_args(mrb, "o", &src);

  if (!mrb_string_p(src)) {
    src = mrb_funcall(mrb, src, "to_s", 0);
  }
  len = RSTRING_LEN(src);

  if (data) {
    memset(data->buf, 0, data->blksz);
    if (len > data->blksz) {
      len = data->blksz;
    }
    memcpy(data->buf, RSTRING_PTR(src), len);
// printf("data=%s, len=%d\n", mrb_str_to_cstr(mrb, src), len);
  }
  return src;
}

/*
 *  call-seq:
 *     mb.data(strip=true)   # => String
 *
 *  Set data to MemoryBuffer.
 *
 *  Parameters:
 *    +data+    Data to be set in the MemoryBuffer
 *
 *  Returns set data.
 */
mrb_value
mrb_mbf_dget(mrb_state *mrb, mrb_value self)
{
  mrb_value buf;
  char *dst;
  mrb_bool strip = 1;
  mrb_mbf_t *data = DATA_PTR(self);
  size_t len = data->blksz;

  if (!data || !data->buf) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "MessageBuffer was already released.");
  }

  mrb_get_args(mrb, "|b", &strip);

  buf = mrb_str_buf_new(mrb, len + 1);
  dst = RSTRING_PTR(buf);

  memcpy(dst, data->buf, len);
  dst[len] = '\0';
  if (strip) {
    len = strlen(dst);
  }
  mrb_str_resize(mrb, buf, len);

  return buf;
}

void
mrb_rtos_memory_init(mrb_state *mrb, struct RClass *rtos)
{
  struct RClass *mpl;
  struct RClass *mbf;

  /* MemoryPool class */
  mpl = mrb_define_class_under(mrb, rtos, "MemoryPool", mrb->object_class);
  MRB_SET_INSTANCE_TT(mpl, MRB_TT_DATA);

  mrb_define_method(mrb, mpl, "initialize", mrb_mpl_init,       MRB_ARGS_OPT(3));
  mrb_define_method(mrb, mpl, "open",       mrb_mpl_open,       MRB_ARGS_REQ(2));

  /* MemoryBuffer class */
  mbf = mrb_define_class_under(mrb, rtos, "MemoryBuffer", mrb->object_class);
  MRB_SET_INSTANCE_TT(mbf, MRB_TT_DATA);

  mrb_define_method(mrb, mbf, "initialize", mrb_mbf_init,       MRB_ARGS_REQ(3));
  mrb_define_method(mrb, mbf, "release",    mrb_mbf_rel,        MRB_ARGS_NONE());
  mrb_define_method(mrb, mbf, "data=",      mrb_mbf_dset,       MRB_ARGS_REQ(1));
  mrb_define_method(mrb, mbf, "data",       mrb_mbf_dget,       MRB_ARGS_OPT(1));
}
