using System;
using System.IO;
using System.Windows.Forms;
using Microsoft.Win32;


#region AssemblyInfo

using System.Reflection;
using System.Runtime.InteropServices;

// General Information about an assembly is controlled through the following 
// set of attributes. Change these attribute values to modify the information
// associated with an assembly.
[assembly: AssemblyTitle("HostsKeeper")]
[assembly: AssemblyDescription("")]
[assembly: AssemblyConfiguration("")]
[assembly: AssemblyCompany("MIPT")]
[assembly: AssemblyProduct("Hooks Security Systems")]
[assembly: AssemblyCopyright("Copyright © Pavel Kryukov 2011")]
[assembly: AssemblyTrademark("")]
[assembly: AssemblyCulture("")]

// Setting ComVisible to false makes the types in this assembly not visible 
// to COM components.  If you need to access a type in this assembly from 
// COM, set the ComVisible attribute to true on that type.
[assembly: ComVisible(false)]

// The following GUID is for the ID of the typelib if this project is exposed to COM
[assembly: Guid("c14913fc-5add-4d56-a1d0-eb7cfb2e91f5")]

// Version information for an assembly consists of the following four values:
//
//      Major Version
//      Minor Version 
//      Build Number
//      Revision
//
// You can specify all the values or you can default the Build and Revision Numbers 
// by using the '*' as shown below:
// [assembly: AssemblyVersion("1.0.*")]
[assembly: AssemblyVersion("1.0.0.0")]
[assembly: AssemblyFileVersion("1.0.0.0")]


#endregion


namespace HostsKeeper
{
    static class Checker
    {
        /// <summary>
        /// Container of hashsum
        /// </summary>
        private static string _hash = "";

        /// <summary>
        /// Path in register to key
        /// </summary>
        private const string RegistryPath = "software\\HooksSecurity";

        /// <summary>
        /// Name of key in register
        /// </summary>
        private const string RegistryKey = "HostsHash";

        /// <summary>
        /// Path to 'hosts' file
        /// </summary>
        static private readonly string HostsPath = String.Format("{0}{1}", Environment.SystemDirectory, @"\drivers\etc\hosts");

        /// <summary>
        /// Shows message box about error
        /// </summary>
        /// <param name="text"></param>
        static void ShowError(string text)
        {
            MessageBox.Show(text, @"HostsKeeper: Error!");
        }

        /// <summary>
        /// Checking hash of hosts file
        /// </summary>
        /// <returns>Returns hash string</returns>
        static private string CheckHash()
        {
            FileStream hosts;
            try
            {
                // Try to open
                hosts = new FileStream(HostsPath, FileMode.Open, FileAccess.Read);
            }
            catch (FileNotFoundException)
            {
                ShowError(@"There's no 'hosts' file at all!");
                throw;
            }
            catch (UnauthorizedAccessException)
            {
                ShowError(@"HostsKeeper should be runned with administrator rights");
                throw;
            }
            // Counting hash
            var retVal = (new System.Security.Cryptography.MD5CryptoServiceProvider()).ComputeHash(hosts);

            hosts.Close();

            // Converting hash to hex-string
            var sb = new System.Text.StringBuilder();
            for (var i = 0; i < retVal.Length; i++)
            {
                sb.Append(retVal[i].ToString("x2"));
            }
            return sb.ToString();
        }

        /// <summary>
        /// Saves hash in registry
        /// </summary>
        static private void SaveHash()
        {
            try
            {
                var saveKey = Registry.LocalMachine.CreateSubKey(RegistryPath);
                if (saveKey == null)
                {
                    throw new ArgumentNullException();
                }
                saveKey.SetValue(RegistryKey, _hash);
                saveKey.Close();
            }
            catch (UnauthorizedAccessException)
            {
                ShowError(@"HostsKeeper should be runned with administrator rights");
                throw;
            }
            catch (ArgumentNullException)
            {
                ShowError(@"Error in saving key in register");
                throw;
            }
        }

        /// <summary>
        /// Main method
        /// </summary>
        /// <param name="sleepTime">Frequency of checking</param>
        static public void Run(int sleepTime)
        {
            // Opening key folder in registry
            var readKey = Registry.LocalMachine.OpenSubKey(RegistryPath);

            if (readKey != null)
            {
                // Loading key from folder.
                _hash = (string)readKey.GetValue(RegistryKey);

                // If there's no needed key, create.
                if (_hash == null)
                {
                    _hash = CheckHash();
                    SaveHash();
                }
                readKey.Close();
            }
            else
            {
                // Else, creating a key and saving.
                _hash = CheckHash();
                SaveHash();
            }

            while (true)
            {
                // Checking hash
                var newHash = CheckHash();

                // Compare
                if (_hash != newHash)
                {
                    // Asking user about happened
                    var result = MessageBox.Show(@"hosts checksum was changed! Was it planned?", @"HostsKeeper: Warning!",
                                                 MessageBoxButtons.YesNo);
                    if (result == DialogResult.Yes)
                    {
                        // If everything is OK, rewrite hash in registry
                        _hash = newHash;
                        SaveHash();
                    }
                    else
                    {
                        // Opening notepad with 'hosts' file
                        var notepad = System.Diagnostics.Process.Start("notepad", HostsPath);
                        if (notepad == null)
                        {
                            ShowError(@"Couldn't load Notepad");
                        }
                        else
                        {
                            notepad.WaitForExit();
                        }
                    }
                }

                // Going sleep to next iteration.
                System.Threading.Thread.Sleep(sleepTime);
            }
        // ReSharper disable FunctionNeverReturns
        }
        // ReSharper restore FunctionNeverReturns
    }

    static class HostsKeeper
    {
        static void Main()
        {
            try
            {
                Checker.Run(6000);
            }
            catch
            {
                return;
            }
        }
    }
}
