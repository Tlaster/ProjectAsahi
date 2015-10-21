using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using FileManager.Model;
using Windows.Storage;
using Windows.Foundation;
using System.Runtime.Serialization.Json;
using System.IO;
using System.Text;
using System.Linq;
using System.IO.Compression;
using System.Runtime.InteropServices.ComTypes;
using System.Text.RegularExpressions;
using Windows.Storage.Streams;
using System.Runtime.InteropServices.WindowsRuntime;

namespace FileManager
{
    public static class Manager
    {
        public static IAsyncOperation<IList<SaveModel>> LoadSaveList() => LoadTask().AsAsyncOperation();
        public static IAsyncAction Save(SaveModel item) => SaveTask(item).AsAsyncAction();
        public static IAsyncAction Delete(SaveModel item) => DeleteTask(item).AsAsyncAction();
        private static async Task DeleteTask(SaveModel item)
        {
            var file = await ApplicationData.Current.LocalFolder.CreateFileAsync("SaveDate.save", CreationCollisionOption.OpenIfExists);
            var jsStr = await FileIO.ReadTextAsync(file, Windows.Storage.Streams.UnicodeEncoding.Utf16LE);
            var items = jsStr != "" ? Deserialize<List<SaveModel>>(jsStr) : new List<SaveModel>();
            var index = items.FindIndex((a) => { return a.SaveIndex == item.SaveIndex; });
            if (index != -1)
            {
                items.RemoveAt(index);
            }
            jsStr = Serialize(items);
            await FileIO.WriteTextAsync(file, jsStr, Windows.Storage.Streams.UnicodeEncoding.Utf16LE);
        }

        private static async Task SaveTask(SaveModel item)
        {
            item.SaveTime = DateTime.Now.ToLocalTime().ToString();
            var file = await ApplicationData.Current.LocalFolder.CreateFileAsync("SaveDate.save", CreationCollisionOption.OpenIfExists);
            var jsStr = await FileIO.ReadTextAsync(file, Windows.Storage.Streams.UnicodeEncoding.Utf16LE);
            var items = jsStr != "" ? Deserialize<List<SaveModel>>(jsStr) : new List<SaveModel>();
            var index = items.FindIndex((a) => { return a.SaveIndex == item.SaveIndex; });
            if (index == -1)
            {
                item.SaveIndex = items.Count + 1;
                items.Add(item);
            }
            else
            {
                items[index] = item;
            }
            jsStr = Serialize(items);
            await FileIO.WriteTextAsync(file, jsStr, Windows.Storage.Streams.UnicodeEncoding.Utf16LE);
        }
        private static async Task<IList<SaveModel>> LoadTask()
        {
            var file = await ApplicationData.Current.LocalFolder.CreateFileAsync("SaveDate.save", CreationCollisionOption.OpenIfExists);
            var jsStr = await FileIO.ReadTextAsync(file, Windows.Storage.Streams.UnicodeEncoding.Utf16LE);
            if (jsStr=="")
            {
                return null;
            }
            else
            {
                return Deserialize<IEnumerable<SaveModel>>(jsStr).ToList();
            }
        }
        private static T Deserialize<T>(string json)
        {
            var bytes = Encoding.Unicode.GetBytes(json);
            using (MemoryStream _Stream = new MemoryStream(bytes))
            {
                var serializer = new DataContractJsonSerializer(typeof(T));
                return (T)serializer.ReadObject(_Stream);
            }
        }

        private static string Serialize<T>(T instance)
        {
            using (MemoryStream stream = new MemoryStream())
            {
                var serializer = new DataContractJsonSerializer(typeof(T));
                serializer.WriteObject(stream, instance);
                stream.Position = 0;
                using (StreamReader reader = new StreamReader(stream))
                {
                    return reader.ReadToEnd();
                }
            }
        }
        /// <summary>
        /// Get file with file name
        /// </summary>
        /// <param name="file">File name</param>
        /// <param name="type">File type</param>
        /// <returns></returns>
        //public static IAsyncOperation<InMemoryRandomAccessStream> GetFileStreamAsync(string file, FileType type) => GetFileStreamTask(file,false,type).AsAsyncOperation();

        /// <summary>
        /// Get file InMenoryRandomAccessStream with file path
        /// </summary>
        /// <param name="filepath">File path</param>
        /// <returns>File InMenoryRandomAccessStream</returns>
        public static IAsyncOperation<IBuffer> GetFileStreamAsync(string filepath) => GetFileStreamTask(filepath).AsAsyncOperation();

        /// <summary>
        /// Get file string with file path
        /// </summary>
        /// <param name="filePath">File path</param>
        /// <returns>string</returns>
        public static IAsyncOperation<string> GetFileStringAsync(string filePath) => GetFileStringTask(filePath).AsAsyncOperation();

        private static async Task<string> GetFileStringTask(string file,bool containPath = false)
        {
            var pattern = @"[A-Z_a-z][A-Z_a-z0-9\.]*";
            Regex regex = new Regex(pattern);
            var matches = regex.Matches(file);
            var path = $"ms-appx:///{matches[0].Value}.zip";
            var zipfile = await StorageFile.GetFileFromApplicationUriAsync(new Uri(path));
            var str = "";
            using (var ziparchive = ZipFile.OpenRead(zipfile.Path))
            {
                var filePathInZip = file.Remove(0, matches[0].Value.Count() + 1);
                using (var stream = ziparchive.GetEntry(filePathInZip).Open())
                {
                    using (StreamReader reader = new StreamReader(stream, Encoding.Unicode))
                    {
                        str = await reader.ReadToEndAsync();
                    }
                }
            }
            return str;
        }

        private static async Task<IBuffer> GetFileStreamTask(string filePath)
        {
            var pattern = @"[A-Z_a-z][A-Z_a-z0-9\.]*";
            Regex regex = new Regex(pattern);
            var matches = regex.Matches(filePath);
            var path = $"ms-appx:///{matches[0].Value}.zip";
            var zipfile = await StorageFile.GetFileFromApplicationUriAsync(new Uri(path));
            using (var ziparchive = new ZipArchive(await zipfile.OpenStreamForReadAsync(), ZipArchiveMode.Read))
            {
                var filePathInZip = filePath.Remove(0, matches[0].Value.Count() + 1);
                using (var stream = ziparchive.GetEntry(filePathInZip).Open())
                {
                    using (MemoryStream ms = new MemoryStream())
                    {
                        await stream.CopyToAsync(ms);
                        return ms.ToArray().AsBuffer();
                    }
                }
            }
        }
    }
}

