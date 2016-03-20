using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using LibRTWrapperETW;
using System.IO;

namespace ETWTestUapp
{
    public static class TraceCoreWrapper
    {
        public static void VerboseCore(
             string m,
             [CallerMemberName] string memberName = "?",
             [CallerFilePath] string srcFile = "?",
             [CallerLineNumber] int srcNum = 0)
        {
            TraceCore.Instance.Verbose("CORE_RT [" + Environment.CurrentManagedThreadId + "]", Path.GetFileName(srcFile), memberName, m);
        }
    }
}
