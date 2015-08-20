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

namespace FileManager
{
    public static class Manager
    {

        public static IAsyncOperation<IList<SaveModel>> LoadSaveList()
        {
            return LoadTask().AsAsyncOperation();
        }
        public static IAsyncAction Save(SaveModel item)
        {
            return SaveTask(item).AsAsyncAction();
        }
        public static IAsyncAction Delete(SaveModel item)
        {
            return DeleteTask(item).AsAsyncAction();
        }
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
    }
}
