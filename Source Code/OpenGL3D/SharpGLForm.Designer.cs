namespace Field3D
{
	partial class SharpGLForm
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.components = new System.ComponentModel.Container();
			this.openGLControl = new SharpGL.OpenGLControl();
			this.contextMenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
			this.showEMSImDataFilesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.showEMFieldFilesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.showDivergenceReportToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripMenuItem4 = new System.Windows.Forms.ToolStripSeparator();
			this.loadField3DToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.loadField3DVectorToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.loadField3DVectorsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
			this.load3DFieldSphereToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.showRadiusIndexToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripMenuItem3 = new System.Windows.Forms.ToolStripSeparator();
			this.resetRotationToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripMenuItem2 = new System.Windows.Forms.ToolStripSeparator();
			this.setDrawingPercentToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.showRotateAndShiftToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.progressBar1 = new System.Windows.Forms.ProgressBar();
			((System.ComponentModel.ISupportInitialize)(this.openGLControl)).BeginInit();
			this.contextMenuStrip1.SuspendLayout();
			this.SuspendLayout();
			// 
			// openGLControl
			// 
			this.openGLControl.ContextMenuStrip = this.contextMenuStrip1;
			this.openGLControl.Dock = System.Windows.Forms.DockStyle.Fill;
			this.openGLControl.DrawFPS = true;
			this.openGLControl.Location = new System.Drawing.Point(0, 0);
			this.openGLControl.Name = "openGLControl";
			this.openGLControl.OpenGLVersion = SharpGL.Version.OpenGLVersion.OpenGL2_1;
			this.openGLControl.RenderContextType = SharpGL.RenderContextType.FBO;
			this.openGLControl.RenderTrigger = SharpGL.RenderTrigger.TimerBased;
			this.openGLControl.Size = new System.Drawing.Size(624, 391);
			this.openGLControl.TabIndex = 0;
			this.openGLControl.OpenGLInitialized += new System.EventHandler(this.openGLControl_OpenGLInitialized);
			this.openGLControl.OpenGLDraw += new SharpGL.RenderEventHandler(this.openGLControl_OpenGLDraw);
			this.openGLControl.Resized += new System.EventHandler(this.openGLControl_Resized);
			this.openGLControl.MouseDown += new System.Windows.Forms.MouseEventHandler(this.openGLControl_MouseDown);
			this.openGLControl.MouseMove += new System.Windows.Forms.MouseEventHandler(this.openGLControl_MouseMove);
			this.openGLControl.MouseUp += new System.Windows.Forms.MouseEventHandler(this.openGLControl_MouseUp);
			// 
			// contextMenuStrip1
			// 
			this.contextMenuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.showEMSImDataFilesToolStripMenuItem,
            this.showEMFieldFilesToolStripMenuItem,
            this.showDivergenceReportToolStripMenuItem,
            this.toolStripMenuItem4,
            this.loadField3DToolStripMenuItem,
            this.loadField3DVectorToolStripMenuItem,
            this.loadField3DVectorsToolStripMenuItem,
            this.toolStripMenuItem1,
            this.load3DFieldSphereToolStripMenuItem,
            this.showRadiusIndexToolStripMenuItem,
            this.toolStripMenuItem3,
            this.resetRotationToolStripMenuItem,
            this.toolStripMenuItem2,
            this.setDrawingPercentToolStripMenuItem,
            this.showRotateAndShiftToolStripMenuItem});
			this.contextMenuStrip1.Name = "contextMenuStrip1";
			this.contextMenuStrip1.Size = new System.Drawing.Size(204, 292);
			// 
			// showEMSImDataFilesToolStripMenuItem
			// 
			this.showEMSImDataFilesToolStripMenuItem.Name = "showEMSImDataFilesToolStripMenuItem";
			this.showEMSImDataFilesToolStripMenuItem.Size = new System.Drawing.Size(203, 22);
			this.showEMSImDataFilesToolStripMenuItem.Text = "Show EM Sim Data Files";
			this.showEMSImDataFilesToolStripMenuItem.ToolTipText = "Load EM Simulation Data Files. Each data file is for one time step. Select one da" +
    "ta file for initial drawing. \r\n";
			this.showEMSImDataFilesToolStripMenuItem.Click += new System.EventHandler(this.showEMSImDataFilesToolStripMenuItem_Click);
			// 
			// showEMFieldFilesToolStripMenuItem
			// 
			this.showEMFieldFilesToolStripMenuItem.Name = "showEMFieldFilesToolStripMenuItem";
			this.showEMFieldFilesToolStripMenuItem.Size = new System.Drawing.Size(203, 22);
			this.showEMFieldFilesToolStripMenuItem.Text = "Show EM Field Files";
			this.showEMFieldFilesToolStripMenuItem.ToolTipText = "Load EM field file, it contains list of 9 doubles: 3 for position, 6 for field";
			this.showEMFieldFilesToolStripMenuItem.Click += new System.EventHandler(this.showEMFieldFilesToolStripMenuItem_Click);
			// 
			// showDivergenceReportToolStripMenuItem
			// 
			this.showDivergenceReportToolStripMenuItem.Name = "showDivergenceReportToolStripMenuItem";
			this.showDivergenceReportToolStripMenuItem.Size = new System.Drawing.Size(203, 22);
			this.showDivergenceReportToolStripMenuItem.Text = "Show Divergence Report";
			this.showDivergenceReportToolStripMenuItem.ToolTipText = "load divergence report file";
			this.showDivergenceReportToolStripMenuItem.Click += new System.EventHandler(this.showDivergenceReportToolStripMenuItem_Click);
			// 
			// toolStripMenuItem4
			// 
			this.toolStripMenuItem4.Name = "toolStripMenuItem4";
			this.toolStripMenuItem4.Size = new System.Drawing.Size(200, 6);
			// 
			// loadField3DToolStripMenuItem
			// 
			this.loadField3DToolStripMenuItem.Name = "loadField3DToolStripMenuItem";
			this.loadField3DToolStripMenuItem.Size = new System.Drawing.Size(203, 22);
			this.loadField3DToolStripMenuItem.Text = "Load Field 3D scalar";
			this.loadField3DToolStripMenuItem.ToolTipText = "Geometry read from *.info.  Data in 3D array of doubles";
			this.loadField3DToolStripMenuItem.Click += new System.EventHandler(this.loadField3DToolStripMenuItem_Click);
			// 
			// loadField3DVectorToolStripMenuItem
			// 
			this.loadField3DVectorToolStripMenuItem.Name = "loadField3DVectorToolStripMenuItem";
			this.loadField3DVectorToolStripMenuItem.Size = new System.Drawing.Size(203, 22);
			this.loadField3DVectorToolStripMenuItem.Text = "Load Field 3D vector";
			this.loadField3DVectorToolStripMenuItem.ToolTipText = "Geometry read from *.info. Data in 3D array of 3D doubles for the ending point of" +
    " a vector starting from the origin.";
			this.loadField3DVectorToolStripMenuItem.Click += new System.EventHandler(this.loadField3DVectorToolStripMenuItem_Click);
			// 
			// loadField3DVectorsToolStripMenuItem
			// 
			this.loadField3DVectorsToolStripMenuItem.Name = "loadField3DVectorsToolStripMenuItem";
			this.loadField3DVectorsToolStripMenuItem.Size = new System.Drawing.Size(203, 22);
			this.loadField3DVectorsToolStripMenuItem.Text = "Load Field 3D Vectors";
			this.loadField3DVectorsToolStripMenuItem.ToolTipText = "Data format: 3D array of 2 3D doubles for the starting and ending of the vector";
			this.loadField3DVectorsToolStripMenuItem.Click += new System.EventHandler(this.loadField3DVectorsToolStripMenuItem_Click);
			// 
			// toolStripMenuItem1
			// 
			this.toolStripMenuItem1.Name = "toolStripMenuItem1";
			this.toolStripMenuItem1.Size = new System.Drawing.Size(200, 6);
			// 
			// load3DFieldSphereToolStripMenuItem
			// 
			this.load3DFieldSphereToolStripMenuItem.Name = "load3DFieldSphereToolStripMenuItem";
			this.load3DFieldSphereToolStripMenuItem.Size = new System.Drawing.Size(203, 22);
			this.load3DFieldSphereToolStripMenuItem.Text = "Load 3D Field Sphere";
			this.load3DFieldSphereToolStripMenuItem.Click += new System.EventHandler(this.load3DFieldSphereToolStripMenuItem_Click);
			// 
			// showRadiusIndexToolStripMenuItem
			// 
			this.showRadiusIndexToolStripMenuItem.Name = "showRadiusIndexToolStripMenuItem";
			this.showRadiusIndexToolStripMenuItem.Size = new System.Drawing.Size(203, 22);
			this.showRadiusIndexToolStripMenuItem.Text = "Show Radius Index";
			this.showRadiusIndexToolStripMenuItem.Click += new System.EventHandler(this.showRadiusIndexToolStripMenuItem_Click);
			// 
			// toolStripMenuItem3
			// 
			this.toolStripMenuItem3.Name = "toolStripMenuItem3";
			this.toolStripMenuItem3.Size = new System.Drawing.Size(200, 6);
			// 
			// resetRotationToolStripMenuItem
			// 
			this.resetRotationToolStripMenuItem.Name = "resetRotationToolStripMenuItem";
			this.resetRotationToolStripMenuItem.Size = new System.Drawing.Size(203, 22);
			this.resetRotationToolStripMenuItem.Text = "Reset rotation";
			this.resetRotationToolStripMenuItem.ToolTipText = "Set rotation angles to 0";
			this.resetRotationToolStripMenuItem.Click += new System.EventHandler(this.resetRotationToolStripMenuItem_Click);
			// 
			// toolStripMenuItem2
			// 
			this.toolStripMenuItem2.Name = "toolStripMenuItem2";
			this.toolStripMenuItem2.Size = new System.Drawing.Size(200, 6);
			// 
			// setDrawingPercentToolStripMenuItem
			// 
			this.setDrawingPercentToolStripMenuItem.Name = "setDrawingPercentToolStripMenuItem";
			this.setDrawingPercentToolStripMenuItem.Size = new System.Drawing.Size(203, 22);
			this.setDrawingPercentToolStripMenuItem.Text = "Set drawing parameters";
			this.setDrawingPercentToolStripMenuItem.Click += new System.EventHandler(this.setDrawingPercentToolStripMenuItem_Click);
			// 
			// showRotateAndShiftToolStripMenuItem
			// 
			this.showRotateAndShiftToolStripMenuItem.Name = "showRotateAndShiftToolStripMenuItem";
			this.showRotateAndShiftToolStripMenuItem.Size = new System.Drawing.Size(203, 22);
			this.showRotateAndShiftToolStripMenuItem.Text = "Show Rotate and Shift";
			this.showRotateAndShiftToolStripMenuItem.Click += new System.EventHandler(this.showRotateAndShiftToolStripMenuItem_Click);
			// 
			// progressBar1
			// 
			this.progressBar1.Location = new System.Drawing.Point(12, 12);
			this.progressBar1.Name = "progressBar1";
			this.progressBar1.Size = new System.Drawing.Size(258, 23);
			this.progressBar1.TabIndex = 1;
			// 
			// SharpGLForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(624, 391);
			this.Controls.Add(this.progressBar1);
			this.Controls.Add(this.openGLControl);
			this.Name = "SharpGLForm";
			this.Text = "3D Drawing";
			this.WindowState = System.Windows.Forms.FormWindowState.Maximized;
			((System.ComponentModel.ISupportInitialize)(this.openGLControl)).EndInit();
			this.contextMenuStrip1.ResumeLayout(false);
			this.ResumeLayout(false);

		}

		#endregion

		private SharpGL.OpenGLControl openGLControl;
		private System.Windows.Forms.ContextMenuStrip contextMenuStrip1;
		private System.Windows.Forms.ToolStripMenuItem loadField3DToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem resetRotationToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem loadField3DVectorToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem loadField3DVectorsToolStripMenuItem;
		private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
		private System.Windows.Forms.ToolStripMenuItem setDrawingPercentToolStripMenuItem;
		private System.Windows.Forms.ToolStripSeparator toolStripMenuItem2;
		private System.Windows.Forms.ProgressBar progressBar1;
		private System.Windows.Forms.ToolStripMenuItem load3DFieldSphereToolStripMenuItem;
		private System.Windows.Forms.ToolStripSeparator toolStripMenuItem3;
		private System.Windows.Forms.ToolStripMenuItem showRadiusIndexToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem showEMSImDataFilesToolStripMenuItem;
		private System.Windows.Forms.ToolStripSeparator toolStripMenuItem4;
		private System.Windows.Forms.ToolStripMenuItem showDivergenceReportToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem showRotateAndShiftToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem showEMFieldFilesToolStripMenuItem;
	}
}

