/**
 * Copyright (C) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
import image from '@ohos.multimedia.image'
import fileio from '@ohos.fileio'
import mediaLibrary from '@ohos.multimedia.mediaLibrary'
import DateTimeUtil from './DateTimeUtil'
import Logger from './Logger'
import promptAction from '@ohos.promptAction';

const TAG: string = '[MediaUtils]'

class MediaUtils {
  async createAndGetFile(context: any) {
    let mediaTest = mediaLibrary.getMediaLibrary(context)
    let info = {
      prefix: 'IMG_', suffix: '.jpg', directory: mediaLibrary.DirectoryType.DIR_IMAGE
    }
    let dateTimeUtil = new DateTimeUtil()
    let name = `${dateTimeUtil.getDate()}_${dateTimeUtil.getTime()}`
    let displayName = `${info.prefix}${name}${info.suffix}`
    let publicPath = await mediaTest.getPublicDirectory(info.directory)
    Logger.info(TAG, `publicPath = ${publicPath}`)
    return await mediaTest.createAsset(mediaLibrary.MediaType.IMAGE, displayName, publicPath)
  }

  async savePicture(data: image.PixelMap, context: any) {
    Logger.info(TAG, `savePicture`)
    let packOpts: image.PackingOption = {
      format: "image/jpeg", quality: 100
    }
    let imagePackerApi = image.createImagePacker()
    let arrayBuffer = await imagePackerApi.packing(data, packOpts)
    let fileAsset = await this.createAndGetFile(context)
    let fd = await fileAsset.open('Rw')
    imagePackerApi.release()
    try {
      await fileio.write(fd, arrayBuffer)
    } catch(err) {
      Logger.error(`write failed, code is ${err.code}, message is ${err.message}`)
    }
    await fileAsset.close(fd)
    Logger.info(TAG, `write done`)
    promptAction.showToast({
      message: '图片保存成功', duration: 1000
    })
  }
}

export default new MediaUtils()