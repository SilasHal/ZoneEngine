using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using System.Runtime.InteropServices.ComTypes;
using System.Text;
using System.Threading.Tasks;
using ZoneEditor.Utilities;

namespace ZoneEditor.GameDev
{
	static class VisualStudio
	{
		private static EnvDTE80.DTE2 _vsInstance = null;
		private static readonly string _progID = "VisualStudio.DTE.17.0";
		[DllImport("ole32.dll")]
		private static extern int CreateBindCtx(uint reserved, out IBindCtx ppbc);

        [DllImport("ole32.dll")]
        private static extern int GetRunningObjectTable(uint reserved, out IRunningObjectTable pprot);
        public static void OpenVisualStudio(string solutionPath)
		{
			IRunningObjectTable rot = null;
			IEnumMoniker monikerTable = null;
			IBindCtx bindCtx = null;
			try
			{
				if (_vsInstance == null) 
				{
					//Find the open visual
					var hResult = GetRunningObjectTable(0, out rot);
					if (hResult < 0 || rot == null) throw new COMException($"GetRunningObjectTable() returned HRESULT: {hResult:X8}");

					rot.EnumRunning(out monikerTable);
					monikerTable.Reset();

					hResult = CreateBindCtx(0, out bindCtx);
                    if (hResult < 0 || bindCtx == null) throw new COMException($"CreateBindCtx() returned HRESULT: {hResult:X8}");

					IMoniker[] currentMoniker = new IMoniker[1];
					while(monikerTable.Next(1,currentMoniker,IntPtr.Zero) == 0)
					{
						string name = string.Empty;
						currentMoniker[0]?.GetDisplayName(bindCtx, null, out name);
						if (name.Contains(_progID))
						{
							hResult = rot.GetObject(currentMoniker[0], out object obj);
                            if (hResult < 0 || obj == null) throw new COMException($"Running object table's GetObject() returned HRESULT: {hResult:X8}");

							EnvDTE80.DTE2 dte = obj as EnvDTE80.DTE2;
							var solutionName = dte.Solution.FullName;
							if(solutionName == solutionPath)
							{
								_vsInstance = dte;
								break;
                            }
                        }
                    }

                    if (_vsInstance == null)
					{
						Type visualStudio = Type.GetTypeFromProgID(_progID, true);
                        _vsInstance = Activator.CreateInstance(visualStudio) as EnvDTE80.DTE2;
                    }
				}
			}
			catch (Exception e)
			{
				Debug.WriteLine(e.Message);
				Logger.Log(MessageType.Error, "Failed to open Visual Studio.");
            }
			finally
			{
                if (monikerTable != null) Marshal.ReleaseComObject(monikerTable);
                if (rot != null) Marshal.ReleaseComObject(rot);
                if (bindCtx != null) Marshal.ReleaseComObject(bindCtx);
            }

        }

        public static void CloseVisualStudio()
        {
            if (_vsInstance?.Solution.IsOpen == null)
            {
                _vsInstance.ExecuteCommand("File.SaveAll");
                _vsInstance.Solution.Close(true);
            }
			_vsInstance.Quit();
            _vsInstance = null;
		}
	}
}
	

