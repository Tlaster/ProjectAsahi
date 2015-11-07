using ICSharpCode.AvalonEdit.CodeCompletion;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ICSharpCode.AvalonEdit.Document;
using ICSharpCode.AvalonEdit.Editing;
using System.Windows.Media;

namespace ScriptWriter.Common
{
    public sealed class CompletionData : ICompletionData
    {
        private Action<CompletionData> _onComplete;
        public CompletionData(Action<CompletionData> onComplete, string text,string des = null)
        {
            _onComplete = onComplete;
            Text = text;
            Description = des;
        }
        public object Content => Text;

        public object Description { get; }

        public ImageSource Image => null;

        public double Priority => 0d;

        public string Text { get; }

        public void Complete(TextArea textArea, ISegment completionSegment, EventArgs insertionRequestEventArgs)
        {
            _onComplete?.Invoke(this);
        }
    }
}
