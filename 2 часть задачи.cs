using System;
using System.Diagnostics;
using System.IO;
using System.Text;
using System.Threading;
using System.Windows.Forms;

namespace Launcher
{
    public partial class MainForm : Form
    {
        private Process process;

        public MainForm()
        {
            InitializeComponent();
        }

        private void btnStart_Click(object sender, EventArgs e)
        {
            string pathToCppExe = @"path\to\your\cpp\executable.exe";
            
            // Запуск процесса выполнения консольной программы
            process = new Process
            {
                StartInfo = new ProcessStartInfo
                {
                    FileName = pathToCppExe,
                    UseShellExecute = false,
                    RedirectStandardOutput = true,
                    CreateNoWindow = true
                },
                EnableRaisingEvents = true
            };

            process.Start();

            // Прослушивание стандартного вывода процесса
            process.OutputDataReceived += (sender, args) =>
            {
                textBoxResult.AppendText($"{args.Data}\n");
            };

            process.BeginOutputReadLine();
        }

        private void btnStop_Click(object sender, EventArgs e)
        {
            if (process != null && process.HasExited == false)
            {
                process.Kill();
            }
        }
    }
}