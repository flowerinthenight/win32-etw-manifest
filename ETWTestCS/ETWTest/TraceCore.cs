using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace ETWTest
{
    public static class TraceCore
    {
        public static bool Verbose(
            string m,
            [CallerMemberName] string memberName = "?",
            [CallerFilePath] string srcFile = "?",
            [CallerLineNumber] int srcNum = 0)
        {
            return JyTrace.ProviderJyTrace.EventWriteSimple("ETWTest", Path.GetFileName(srcFile), memberName, "Trace:", m);
        }
    }
}
