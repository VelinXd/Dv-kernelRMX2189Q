/*
* Copyright (c) 2016 MediaTek Inc.
* Author: PC Chen <pc.chen@mediatek.com>
<<<<<<< HEAD
*       Tiffany Lin <tiffany.lin@mediatek.com>
=======
*	Tiffany Lin <tiffany.lin@mediatek.com>
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 2 as
* published by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*/

#include <linux/slab.h>
<<<<<<< HEAD
=======
#include <linux/interrupt.h>
#include <linux/irq.h>
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
#include <linux/module.h>
#include <linux/of_device.h>
#include <linux/of.h>
#include <media/v4l2-event.h>
#include <media/v4l2-mem2mem.h>
#include <media/videobuf2-dma-contig.h>
#include <linux/pm_runtime.h>
<<<<<<< HEAD
#include <linux/pm_wakeup.h>
#include <linux/iommu.h>
#include <linux/delay.h>
#include <linux/suspend.h>
#include <linux/semaphore.h>
=======
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc

#include "mtk_vcodec_drv.h"
#include "mtk_vcodec_enc.h"
#include "mtk_vcodec_enc_pm.h"
#include "mtk_vcodec_intr.h"
#include "mtk_vcodec_util.h"
<<<<<<< HEAD
#include "mtk_vcu.h"

module_param(mtk_v4l2_dbg_level, int, S_IRUGO | S_IWUSR);
module_param(mtk_vcodec_dbg, bool, S_IRUGO | S_IWUSR);
struct mtk_vcodec_dev *venc_dev;
=======
#include "mtk_vpu.h"

module_param(mtk_v4l2_dbg_level, int, S_IRUGO | S_IWUSR);
module_param(mtk_vcodec_dbg, bool, S_IRUGO | S_IWUSR);

/* Wake up context wait_queue */
static void wake_up_ctx(struct mtk_vcodec_ctx *ctx, unsigned int reason)
{
	ctx->int_cond = 1;
	ctx->int_type = reason;
	wake_up_interruptible(&ctx->queue);
}

static void clean_irq_status(unsigned int irq_status, void __iomem *addr)
{
	if (irq_status & MTK_VENC_IRQ_STATUS_PAUSE)
		writel(MTK_VENC_IRQ_STATUS_PAUSE, addr);

	if (irq_status & MTK_VENC_IRQ_STATUS_SWITCH)
		writel(MTK_VENC_IRQ_STATUS_SWITCH, addr);

	if (irq_status & MTK_VENC_IRQ_STATUS_DRAM)
		writel(MTK_VENC_IRQ_STATUS_DRAM, addr);

	if (irq_status & MTK_VENC_IRQ_STATUS_SPS)
		writel(MTK_VENC_IRQ_STATUS_SPS, addr);

	if (irq_status & MTK_VENC_IRQ_STATUS_PPS)
		writel(MTK_VENC_IRQ_STATUS_PPS, addr);

	if (irq_status & MTK_VENC_IRQ_STATUS_FRM)
		writel(MTK_VENC_IRQ_STATUS_FRM, addr);

}
static irqreturn_t mtk_vcodec_enc_irq_handler(int irq, void *priv)
{
	struct mtk_vcodec_dev *dev = priv;
	struct mtk_vcodec_ctx *ctx;
	unsigned long flags;
	void __iomem *addr;

	spin_lock_irqsave(&dev->irqlock, flags);
	ctx = dev->curr_ctx;
	spin_unlock_irqrestore(&dev->irqlock, flags);

	mtk_v4l2_debug(1, "id=%d", ctx->id);
	addr = dev->reg_base[VENC_SYS] + MTK_VENC_IRQ_ACK_OFFSET;

	ctx->irq_status = readl(dev->reg_base[VENC_SYS] +
				(MTK_VENC_IRQ_STATUS_OFFSET));

	clean_irq_status(ctx->irq_status, addr);

	wake_up_ctx(ctx, MTK_INST_IRQ_RECEIVED);
	return IRQ_HANDLED;
}

static irqreturn_t mtk_vcodec_enc_lt_irq_handler(int irq, void *priv)
{
	struct mtk_vcodec_dev *dev = priv;
	struct mtk_vcodec_ctx *ctx;
	unsigned long flags;
	void __iomem *addr;

	spin_lock_irqsave(&dev->irqlock, flags);
	ctx = dev->curr_ctx;
	spin_unlock_irqrestore(&dev->irqlock, flags);

	mtk_v4l2_debug(1, "id=%d", ctx->id);
	ctx->irq_status = readl(dev->reg_base[VENC_LT_SYS] +
				(MTK_VENC_IRQ_STATUS_OFFSET));

	addr = dev->reg_base[VENC_LT_SYS] + MTK_VENC_IRQ_ACK_OFFSET;

	clean_irq_status(ctx->irq_status, addr);

	wake_up_ctx(ctx, MTK_INST_IRQ_RECEIVED);
	return IRQ_HANDLED;
}

static void mtk_vcodec_enc_reset_handler(void *priv)
{
	struct mtk_vcodec_dev *dev = priv;
	struct mtk_vcodec_ctx *ctx;

	mtk_v4l2_debug(0, "Watchdog timeout!!");

	mutex_lock(&dev->dev_mutex);
	list_for_each_entry(ctx, &dev->ctx_list, list) {
		ctx->state = MTK_STATE_ABORT;
		mtk_v4l2_debug(0, "[%d] Change to state MTK_STATE_ABORT",
				ctx->id);
	}
	mutex_unlock(&dev->dev_mutex);
}
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc

static int fops_vcodec_open(struct file *file)
{
	struct mtk_vcodec_dev *dev = video_drvdata(file);
	struct mtk_vcodec_ctx *ctx = NULL;
<<<<<<< HEAD
	struct mtk_video_enc_buf *mtk_buf = NULL;
	struct vb2_queue *src_vq;
=======
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	int ret = 0;

	ctx = kzalloc(sizeof(*ctx), GFP_KERNEL);
	if (!ctx)
		return -ENOMEM;
<<<<<<< HEAD
	mtk_buf = kzalloc(sizeof(*mtk_buf), GFP_KERNEL);
	if (!mtk_buf) {
		kfree(ctx);
		return -ENOMEM;
	}
=======
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc

	mutex_lock(&dev->dev_mutex);
	/*
	 * Use simple counter to uniquely identify this context. Only
	 * used for logging.
	 */
<<<<<<< HEAD
	ctx->enc_flush_buf = mtk_buf;
=======
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	ctx->id = dev->id_counter++;
	v4l2_fh_init(&ctx->fh, video_devdata(file));
	file->private_data = &ctx->fh;
	v4l2_fh_add(&ctx->fh);
	INIT_LIST_HEAD(&ctx->list);
	ctx->dev = dev;
	init_waitqueue_head(&ctx->queue);
<<<<<<< HEAD
	mutex_init(&ctx->worker_lock);
=======
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc

	ctx->type = MTK_INST_ENCODER;
	ret = mtk_vcodec_enc_ctrls_setup(ctx);
	if (ret) {
		mtk_v4l2_err("Failed to setup controls() (%d)",
<<<<<<< HEAD
					 ret);
		goto err_ctrls_setup;
	}
	ctx->m2m_ctx = v4l2_m2m_ctx_init(dev->m2m_dev_enc, ctx,
		&mtk_vcodec_enc_queue_init);
	if (IS_ERR((__force void *)ctx->m2m_ctx)) {
		ret = PTR_ERR((__force void *)ctx->m2m_ctx);
		mtk_v4l2_err("Failed to v4l2_m2m_ctx_init() (%d)",
					 ret);
		goto err_m2m_ctx_init;
	}
	src_vq = v4l2_m2m_get_vq(ctx->m2m_ctx,
		V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE);
	ctx->enc_flush_buf->vb.vb2_buf.vb2_queue = src_vq;
	ctx->enc_flush_buf->lastframe = EOS;
	ctx->enc_flush_buf->vb.vb2_buf.planes[0].bytesused = 1;
=======
				ret);
		goto err_ctrls_setup;
	}
	ctx->m2m_ctx = v4l2_m2m_ctx_init(dev->m2m_dev_enc, ctx,
				&mtk_vcodec_enc_queue_init);
	if (IS_ERR((__force void *)ctx->m2m_ctx)) {
		ret = PTR_ERR((__force void *)ctx->m2m_ctx);
		mtk_v4l2_err("Failed to v4l2_m2m_ctx_init() (%d)",
				ret);
		goto err_m2m_ctx_init;
	}
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	mtk_vcodec_enc_set_default_params(ctx);

	if (v4l2_fh_is_singular(&ctx->fh)) {
		/*
<<<<<<< HEAD
		 * vcu_load_firmware checks if it was loaded already and
		 * does nothing in that case
		 */
		ret = vcu_load_firmware(dev->vcu_plat_dev);
=======
		 * vpu_load_firmware checks if it was loaded already and
		 * does nothing in that case
		 */
		ret = vpu_load_firmware(dev->vpu_plat_dev);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
		if (ret < 0) {
			/*
			 * Return 0 if downloading firmware successfully,
			 * otherwise it is failed
			 */
<<<<<<< HEAD
			mtk_v4l2_err("vcu_load_firmware failed!");
=======
			mtk_v4l2_err("vpu_load_firmware failed!");
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
			goto err_load_fw;
		}

		dev->enc_capability =
<<<<<<< HEAD
			vcu_get_venc_hw_capa(dev->vcu_plat_dev);
=======
			vpu_get_venc_hw_capa(dev->vpu_plat_dev);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
		mtk_v4l2_debug(0, "encoder capability %x", dev->enc_capability);
	}

	mtk_v4l2_debug(2, "Create instance [%d]@%p m2m_ctx=%p ",
<<<<<<< HEAD
				   ctx->id, ctx, ctx->m2m_ctx);

=======
			ctx->id, ctx, ctx->m2m_ctx);

	dev->num_instances++;
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	list_add(&ctx->list, &dev->ctx_list);

	mutex_unlock(&dev->dev_mutex);
	mtk_v4l2_debug(0, "%s encoder [%d]", dev_name(&dev->plat_dev->dev),
<<<<<<< HEAD
				   ctx->id);
=======
			ctx->id);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	return ret;

	/* Deinit when failure occurred */
err_load_fw:
	v4l2_m2m_ctx_release(ctx->m2m_ctx);
err_m2m_ctx_init:
	v4l2_ctrl_handler_free(&ctx->ctrl_hdl);
err_ctrls_setup:
	v4l2_fh_del(&ctx->fh);
	v4l2_fh_exit(&ctx->fh);
<<<<<<< HEAD
	kfree(ctx->enc_flush_buf);
=======
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	kfree(ctx);
	mutex_unlock(&dev->dev_mutex);

	return ret;
}

static int fops_vcodec_release(struct file *file)
{
	struct mtk_vcodec_dev *dev = video_drvdata(file);
	struct mtk_vcodec_ctx *ctx = fh_to_ctx(file->private_data);

<<<<<<< HEAD
	mtk_v4l2_debug(0, "[%d] encoder", ctx->id);
	mutex_lock(&dev->dev_mutex);

	mtk_release_pmqos(ctx);
	mtk_vcodec_enc_empty_queues(file, ctx);
	mutex_lock(&ctx->worker_lock);
	v4l2_m2m_ctx_release(ctx->m2m_ctx);
	mutex_unlock(&ctx->worker_lock);
=======
	mtk_v4l2_debug(1, "[%d] encoder", ctx->id);
	mutex_lock(&dev->dev_mutex);

	/*
	 * Call v4l2_m2m_ctx_release to make sure the worker thread is not
	 * running after venc_if_deinit.
	 */
	v4l2_m2m_ctx_release(ctx->m2m_ctx);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	mtk_vcodec_enc_release(ctx);
	v4l2_fh_del(&ctx->fh);
	v4l2_fh_exit(&ctx->fh);
	v4l2_ctrl_handler_free(&ctx->ctrl_hdl);

	list_del_init(&ctx->list);
<<<<<<< HEAD
	kfree(ctx->enc_flush_buf);
=======
	dev->num_instances--;
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	kfree(ctx);
	mutex_unlock(&dev->dev_mutex);
	return 0;
}

static const struct v4l2_file_operations mtk_vcodec_fops = {
<<<<<<< HEAD
	.owner          = THIS_MODULE,
	.open           = fops_vcodec_open,
	.release        = fops_vcodec_release,
	.poll           = v4l2_m2m_fop_poll,
	.unlocked_ioctl = video_ioctl2,
	.mmap           = v4l2_m2m_fop_mmap,
};

/**
 * Suspsend callbacks after user space processes are frozen
 * Since user space processes are frozen, there is no need and cannot hold same
 * mutex that protects lock owner while checking status.
 * If video codec hardware is still active now, must not to enter suspend.
 **/
static int mtk_vcodec_enc_suspend(struct device *pDev)
{
	int val = 0;

	val = down_trylock(&venc_dev->enc_sem);
	if (val == 1) {
		mtk_v4l2_debug(0, "fail due to videocodec activity");
		return -EBUSY;
	}
	up(&venc_dev->enc_sem);

	mtk_v4l2_debug(1, "done");
	return 0;
}

static int mtk_vcodec_enc_resume(struct device *pDev)
{
	mtk_v4l2_debug(1, "done");
	return 0;
}

static int mtk_vcodec_enc_suspend_notifier(struct notifier_block *nb,
					unsigned long action, void *data)
{
	int wait_cnt = 0;
	int val = 0;

	mtk_v4l2_debug(1, "action = %ld", action);
	switch (action) {
	case PM_SUSPEND_PREPARE:
		venc_dev->is_codec_suspending = 1;
		do {
			usleep_range(10000, 20000);
			wait_cnt++;
			if (wait_cnt > 5) {
				mtk_v4l2_err("waiting fail");
				/* Current task is still not finished, don't
				 * care, will check again in real suspend
				 */
				return NOTIFY_DONE;
			}
			val = down_trylock(&venc_dev->enc_sem);
		} while (val == 1);
		up(&venc_dev->enc_sem);

		return NOTIFY_OK;
	case PM_POST_SUSPEND:
		venc_dev->is_codec_suspending = 0;
		return NOTIFY_OK;
	default:
		return NOTIFY_DONE;
	}
	return NOTIFY_DONE;
}


static int mtk_vcodec_enc_probe(struct platform_device *pdev)
=======
	.owner		= THIS_MODULE,
	.open		= fops_vcodec_open,
	.release	= fops_vcodec_release,
	.poll		= v4l2_m2m_fop_poll,
	.unlocked_ioctl	= video_ioctl2,
	.mmap		= v4l2_m2m_fop_mmap,
};

static int mtk_vcodec_probe(struct platform_device *pdev)
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
{
	struct mtk_vcodec_dev *dev;
	struct video_device *vfd_enc;
	struct resource *res;
<<<<<<< HEAD
	struct mtk_vcodec_pm *pm;
	int i, ret;
=======
	int i, j, ret;
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc

	dev = devm_kzalloc(&pdev->dev, sizeof(*dev), GFP_KERNEL);
	if (!dev)
		return -ENOMEM;

	INIT_LIST_HEAD(&dev->ctx_list);
	dev->plat_dev = pdev;

<<<<<<< HEAD
	dev->vcu_plat_dev = vcu_get_plat_device(dev->plat_dev);
	if (dev->vcu_plat_dev == NULL) {
		mtk_v4l2_err("[VCU] vcu device in not ready");
		return -EPROBE_DEFER;
	}

=======
	dev->vpu_plat_dev = vpu_get_plat_device(dev->plat_dev);
	if (dev->vpu_plat_dev == NULL) {
		mtk_v4l2_err("[VPU] vpu device in not ready");
		return -EPROBE_DEFER;
	}

	vpu_wdt_reg_handler(dev->vpu_plat_dev, mtk_vcodec_enc_reset_handler,
				dev, VPU_RST_ENC);

>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	ret = mtk_vcodec_init_enc_pm(dev);
	if (ret < 0) {
		dev_err(&pdev->dev, "Failed to get mt vcodec clock source!");
		return ret;
	}

<<<<<<< HEAD
	pm = &dev->pm;
	pm->chip_node = of_find_compatible_node(NULL,
		NULL, "mediatek,venc_gcon");
	if (pm->chip_node) {
		for (i = VENC_SYS; i < NUM_MAX_VENC_REG_BASE; i++) {
			res = platform_get_resource(pdev, IORESOURCE_MEM, i);
			if (res == NULL) {
				dev_err(&pdev->dev, "get memory resource failed.");
				ret = -ENXIO;
				goto err_res;
			}
			dev->enc_reg_base[i] =
				devm_ioremap_resource(&pdev->dev, res);
			if (IS_ERR((__force void *)dev->enc_reg_base[i])) {
				ret = PTR_ERR(
					(__force void *)dev->enc_reg_base[i]);
				goto err_res;
			}
			mtk_v4l2_debug(2, "reg[%d] base=0x%p",
				i, dev->enc_reg_base[i]);
		}
	} else {
		res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
=======
	for (i = VENC_SYS, j = 0; i < NUM_MAX_VCODEC_REG_BASE; i++, j++) {
		res = platform_get_resource(pdev, IORESOURCE_MEM, j);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
		if (res == NULL) {
			dev_err(&pdev->dev, "get memory resource failed.");
			ret = -ENXIO;
			goto err_res;
		}
<<<<<<< HEAD
		dev->enc_reg_base[VENC_SYS] =
			devm_ioremap_resource(&pdev->dev, res);
		if (IS_ERR((__force void *)dev->enc_reg_base[VENC_SYS])) {
			ret = PTR_ERR(
				(__force void *)dev->enc_reg_base[VENC_SYS]);
			goto err_res;
		}
		mtk_v4l2_debug(2, "reg[%d] base=0x%p",
			VENC_SYS, dev->enc_reg_base[VENC_SYS]);
=======
		dev->reg_base[i] = devm_ioremap_resource(&pdev->dev, res);
		if (IS_ERR((__force void *)dev->reg_base[i])) {
			ret = PTR_ERR((__force void *)dev->reg_base[i]);
			goto err_res;
		}
		mtk_v4l2_debug(2, "reg[%d] base=0x%p", i, dev->reg_base[i]);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	}

	res = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	if (res == NULL) {
		dev_err(&pdev->dev, "failed to get irq resource");
		ret = -ENOENT;
		goto err_res;
	}

<<<<<<< HEAD
	ret = mtk_vcodec_enc_irq_setup(pdev, dev);
	if (ret)
		goto err_res;

	sema_init(&dev->enc_sem, 1);
	mutex_init(&dev->dev_mutex);
	mutex_init(&dev->enc_dvfs_mutex);
	spin_lock_init(&dev->irqlock);

	snprintf(dev->v4l2_dev.name, sizeof(dev->v4l2_dev.name), "%s",
			 "[MTK_V4L2_VENC]");
=======
	dev->enc_irq = platform_get_irq(pdev, 0);
	ret = devm_request_irq(&pdev->dev, dev->enc_irq,
			       mtk_vcodec_enc_irq_handler,
			       0, pdev->name, dev);
	if (ret) {
		dev_err(&pdev->dev, "Failed to install dev->enc_irq %d (%d)",
			dev->enc_irq,
			ret);
		ret = -EINVAL;
		goto err_res;
	}

	dev->enc_lt_irq = platform_get_irq(pdev, 1);
	ret = devm_request_irq(&pdev->dev,
			       dev->enc_lt_irq, mtk_vcodec_enc_lt_irq_handler,
			       0, pdev->name, dev);
	if (ret) {
		dev_err(&pdev->dev,
			"Failed to install dev->enc_lt_irq %d (%d)",
			dev->enc_lt_irq, ret);
		ret = -EINVAL;
		goto err_res;
	}

	disable_irq(dev->enc_irq);
	disable_irq(dev->enc_lt_irq); /* VENC_LT */
	mutex_init(&dev->enc_mutex);
	mutex_init(&dev->dev_mutex);
	spin_lock_init(&dev->irqlock);

	snprintf(dev->v4l2_dev.name, sizeof(dev->v4l2_dev.name), "%s",
		 "[MTK_V4L2_VENC]");
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc

	ret = v4l2_device_register(&pdev->dev, &dev->v4l2_dev);
	if (ret) {
		mtk_v4l2_err("v4l2_device_register err=%d", ret);
		goto err_res;
	}

	init_waitqueue_head(&dev->queue);

	/* allocate video device for encoder and register it */
	vfd_enc = video_device_alloc();
	if (!vfd_enc) {
		mtk_v4l2_err("Failed to allocate video device");
		ret = -ENOMEM;
		goto err_enc_alloc;
	}
	vfd_enc->fops           = &mtk_vcodec_fops;
	vfd_enc->ioctl_ops      = &mtk_venc_ioctl_ops;
	vfd_enc->release        = video_device_release;
	vfd_enc->lock           = &dev->dev_mutex;
	vfd_enc->v4l2_dev       = &dev->v4l2_dev;
	vfd_enc->vfl_dir        = VFL_DIR_M2M;
<<<<<<< HEAD
	vfd_enc->device_caps    = V4L2_CAP_VIDEO_M2M_MPLANE |
							  V4L2_CAP_STREAMING;

	snprintf(vfd_enc->name, sizeof(vfd_enc->name), "%s",
			 MTK_VCODEC_ENC_NAME);
=======
	vfd_enc->device_caps	= V4L2_CAP_VIDEO_M2M_MPLANE |
					V4L2_CAP_STREAMING;

	snprintf(vfd_enc->name, sizeof(vfd_enc->name), "%s",
		 MTK_VCODEC_ENC_NAME);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	video_set_drvdata(vfd_enc, dev);
	dev->vfd_enc = vfd_enc;
	platform_set_drvdata(pdev, dev);

	dev->m2m_dev_enc = v4l2_m2m_init(&mtk_venc_m2m_ops);
	if (IS_ERR((__force void *)dev->m2m_dev_enc)) {
		mtk_v4l2_err("Failed to init mem2mem enc device");
		ret = PTR_ERR((__force void *)dev->m2m_dev_enc);
		goto err_enc_mem_init;
	}

	dev->encode_workqueue =
<<<<<<< HEAD
		alloc_ordered_workqueue(MTK_VCODEC_ENC_NAME,
								WQ_MEM_RECLAIM |
								WQ_FREEZABLE);
=======
			alloc_ordered_workqueue(MTK_VCODEC_ENC_NAME,
						WQ_MEM_RECLAIM |
						WQ_FREEZABLE);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	if (!dev->encode_workqueue) {
		mtk_v4l2_err("Failed to create encode workqueue");
		ret = -EINVAL;
		goto err_event_workq;
	}

	ret = video_register_device(vfd_enc, VFL_TYPE_GRABBER, 1);
	if (ret) {
		mtk_v4l2_err("Failed to register video device");
		goto err_enc_reg;
	}

	mtk_v4l2_debug(0, "encoder registered as /dev/video%d",
<<<<<<< HEAD
				   vfd_enc->num);

	mtk_prepare_venc_dvfs();
	mtk_prepare_venc_emi_bw();
	pm_notifier(mtk_vcodec_enc_suspend_notifier, 0);
	dev->is_codec_suspending = 0;
	venc_dev = dev;
=======
			vfd_enc->num);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc

	return 0;

err_enc_reg:
	destroy_workqueue(dev->encode_workqueue);
err_event_workq:
	v4l2_m2m_release(dev->m2m_dev_enc);
err_enc_mem_init:
	video_unregister_device(vfd_enc);
err_enc_alloc:
	v4l2_device_unregister(&dev->v4l2_dev);
err_res:
	mtk_vcodec_release_enc_pm(dev);
	return ret;
}

static const struct of_device_id mtk_vcodec_enc_match[] = {
	{.compatible = "mediatek,mt8173-vcodec-enc",},
<<<<<<< HEAD
	{.compatible = "mediatek,mt2712-vcodec-enc",},
	{.compatible = "mediatek,mt8167-vcodec-enc",},
	{.compatible = "mediatek,mt6771-vcodec-enc",},
	{.compatible = "mediatek,venc_gcon",},
=======
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	{},
};
MODULE_DEVICE_TABLE(of, mtk_vcodec_enc_match);

static int mtk_vcodec_enc_remove(struct platform_device *pdev)
{
	struct mtk_vcodec_dev *dev = platform_get_drvdata(pdev);

<<<<<<< HEAD
	mtk_unprepare_venc_emi_bw();
	mtk_unprepare_venc_dvfs();

=======
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	mtk_v4l2_debug_enter();
	flush_workqueue(dev->encode_workqueue);
	destroy_workqueue(dev->encode_workqueue);
	if (dev->m2m_dev_enc)
		v4l2_m2m_release(dev->m2m_dev_enc);

	if (dev->vfd_enc)
		video_unregister_device(dev->vfd_enc);

	v4l2_device_unregister(&dev->v4l2_dev);
	mtk_vcodec_release_enc_pm(dev);
	return 0;
}

<<<<<<< HEAD
static const struct dev_pm_ops mtk_vcodec_enc_pm_ops = {
	.suspend = mtk_vcodec_enc_suspend,
	.resume = mtk_vcodec_enc_resume,
};

static struct platform_driver mtk_vcodec_enc_driver = {
	.probe  = mtk_vcodec_enc_probe,
	.remove = mtk_vcodec_enc_remove,
	.driver = {
		.name   = MTK_VCODEC_ENC_NAME,
		.pm = &mtk_vcodec_enc_pm_ops,
=======
static struct platform_driver mtk_vcodec_enc_driver = {
	.probe	= mtk_vcodec_probe,
	.remove	= mtk_vcodec_enc_remove,
	.driver	= {
		.name	= MTK_VCODEC_ENC_NAME,
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
		.of_match_table = mtk_vcodec_enc_match,
	},
};

module_platform_driver(mtk_vcodec_enc_driver);


MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("Mediatek video codec V4L2 encoder driver");
