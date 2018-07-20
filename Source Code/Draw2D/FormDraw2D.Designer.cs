namespace Draw2D
{
	partial class FormDraw2D
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
			this.contextMenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
			this.eMErrorDifferencesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.addATimefileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripMenuItem3 = new System.Windows.Forms.ToolStripSeparator();
			this.showDataFileInChartToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.fFTDataFilesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.dComplexToComplexToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.dRealToComplexToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.dComplexToComplexToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
			this.dRealToComplexToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
			this.showChartFFTDataFilesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.dComplexFileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.dStrengthToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.dRealSourceInFrequencyToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.dStrengthToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
			this.dStrengthNormalizedToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.dStrengthNormalizedByDimToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.dRelativeStrengthDecreaseToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.showChartFFTAnalysisToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.dSpaceFrequencyIndexToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.dMaxcenterFrequenciesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.showCoseToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
			this.doublesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.highLowRowsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripMenuItem2 = new System.Windows.Forms.ToolStripSeparator();
			this.showDataControlToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.eMPerformancesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.contextMenuStrip1.SuspendLayout();
			this.SuspendLayout();
			// 
			// contextMenuStrip1
			// 
			this.contextMenuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.eMErrorDifferencesToolStripMenuItem,
            this.eMPerformancesToolStripMenuItem,
            this.toolStripMenuItem3,
            this.showDataFileInChartToolStripMenuItem,
            this.fFTDataFilesToolStripMenuItem,
            this.showChartFFTDataFilesToolStripMenuItem,
            this.showChartFFTAnalysisToolStripMenuItem,
            this.showCoseToolStripMenuItem,
            this.toolStripMenuItem1,
            this.doublesToolStripMenuItem,
            this.addATimefileToolStripMenuItem,
            this.highLowRowsToolStripMenuItem,
            this.toolStripMenuItem2,
            this.showDataControlToolStripMenuItem});
			this.contextMenuStrip1.Name = "contextMenuStrip1";
			this.contextMenuStrip1.Size = new System.Drawing.Size(216, 286);
			// 
			// eMErrorDifferencesToolStripMenuItem
			// 
			this.eMErrorDifferencesToolStripMenuItem.Name = "eMErrorDifferencesToolStripMenuItem";
			this.eMErrorDifferencesToolStripMenuItem.Size = new System.Drawing.Size(215, 22);
			this.eMErrorDifferencesToolStripMenuItem.Text = "EM Divergence Differences";
			this.eMErrorDifferencesToolStripMenuItem.Click += new System.EventHandler(this.eMErrorDifferencesToolStripMenuItem_Click);
			// 
			// addATimefileToolStripMenuItem
			// 
			this.addATimefileToolStripMenuItem.Name = "addATimefileToolStripMenuItem";
			this.addATimefileToolStripMenuItem.Size = new System.Drawing.Size(215, 22);
			this.addATimefileToolStripMenuItem.Text = "Add a time-file";
			this.addATimefileToolStripMenuItem.Click += new System.EventHandler(this.addATimefileToolStripMenuItem_Click);
			// 
			// toolStripMenuItem3
			// 
			this.toolStripMenuItem3.Name = "toolStripMenuItem3";
			this.toolStripMenuItem3.Size = new System.Drawing.Size(212, 6);
			// 
			// showDataFileInChartToolStripMenuItem
			// 
			this.showDataFileInChartToolStripMenuItem.Name = "showDataFileInChartToolStripMenuItem";
			this.showDataFileInChartToolStripMenuItem.Size = new System.Drawing.Size(215, 22);
			this.showDataFileInChartToolStripMenuItem.Text = "Data File in Chart";
			this.showDataFileInChartToolStripMenuItem.ToolTipText = "File format: double (variable), double (magnitude)";
			this.showDataFileInChartToolStripMenuItem.Click += new System.EventHandler(this.showDataFileInChartToolStripMenuItem_Click);
			// 
			// fFTDataFilesToolStripMenuItem
			// 
			this.fFTDataFilesToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.dComplexToComplexToolStripMenuItem,
            this.dRealToComplexToolStripMenuItem,
            this.dComplexToComplexToolStripMenuItem1,
            this.dRealToComplexToolStripMenuItem1});
			this.fFTDataFilesToolStripMenuItem.Name = "fFTDataFilesToolStripMenuItem";
			this.fFTDataFilesToolStripMenuItem.Size = new System.Drawing.Size(215, 22);
			this.fFTDataFilesToolStripMenuItem.Text = "FFT File (Text)";
			// 
			// dComplexToComplexToolStripMenuItem
			// 
			this.dComplexToComplexToolStripMenuItem.Name = "dComplexToComplexToolStripMenuItem";
			this.dComplexToComplexToolStripMenuItem.Size = new System.Drawing.Size(202, 22);
			this.dComplexToComplexToolStripMenuItem.Text = "1D Complex to Complex";
			this.dComplexToComplexToolStripMenuItem.Click += new System.EventHandler(this.dComplexToComplexToolStripMenuItem_Click);
			// 
			// dRealToComplexToolStripMenuItem
			// 
			this.dRealToComplexToolStripMenuItem.Name = "dRealToComplexToolStripMenuItem";
			this.dRealToComplexToolStripMenuItem.Size = new System.Drawing.Size(202, 22);
			this.dRealToComplexToolStripMenuItem.Text = "1D Real to Complex";
			this.dRealToComplexToolStripMenuItem.Click += new System.EventHandler(this.dRealToComplexToolStripMenuItem_Click);
			// 
			// dComplexToComplexToolStripMenuItem1
			// 
			this.dComplexToComplexToolStripMenuItem1.Name = "dComplexToComplexToolStripMenuItem1";
			this.dComplexToComplexToolStripMenuItem1.Size = new System.Drawing.Size(202, 22);
			this.dComplexToComplexToolStripMenuItem1.Text = "2D Complex to Complex";
			this.dComplexToComplexToolStripMenuItem1.Click += new System.EventHandler(this.dComplexToComplexToolStripMenuItem1_Click);
			// 
			// dRealToComplexToolStripMenuItem1
			// 
			this.dRealToComplexToolStripMenuItem1.Name = "dRealToComplexToolStripMenuItem1";
			this.dRealToComplexToolStripMenuItem1.Size = new System.Drawing.Size(202, 22);
			this.dRealToComplexToolStripMenuItem1.Text = "2D Real to Complex";
			this.dRealToComplexToolStripMenuItem1.Click += new System.EventHandler(this.dRealToComplexToolStripMenuItem1_Click);
			// 
			// showChartFFTDataFilesToolStripMenuItem
			// 
			this.showChartFFTDataFilesToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.dComplexFileToolStripMenuItem,
            this.dStrengthToolStripMenuItem,
            this.dRealSourceInFrequencyToolStripMenuItem,
            this.dStrengthToolStripMenuItem1,
            this.dStrengthNormalizedToolStripMenuItem,
            this.dStrengthNormalizedByDimToolStripMenuItem,
            this.dRelativeStrengthDecreaseToolStripMenuItem});
			this.showChartFFTDataFilesToolStripMenuItem.Name = "showChartFFTDataFilesToolStripMenuItem";
			this.showChartFFTDataFilesToolStripMenuItem.Size = new System.Drawing.Size(215, 22);
			this.showChartFFTDataFilesToolStripMenuItem.Text = "FFT File (Chart)";
			// 
			// dComplexFileToolStripMenuItem
			// 
			this.dComplexFileToolStripMenuItem.Name = "dComplexFileToolStripMenuItem";
			this.dComplexFileToolStripMenuItem.Size = new System.Drawing.Size(241, 22);
			this.dComplexFileToolStripMenuItem.Text = "1D Complex Source";
			this.dComplexFileToolStripMenuItem.ToolTipText = "Data from complex source to Complex frequency FFT. File format: unsigned (frequen" +
    "cy index), double (real),  double (imaginary)";
			this.dComplexFileToolStripMenuItem.Click += new System.EventHandler(this.dComplexFileToolStripMenuItem_Click);
			// 
			// dStrengthToolStripMenuItem
			// 
			this.dStrengthToolStripMenuItem.Name = "dStrengthToolStripMenuItem";
			this.dStrengthToolStripMenuItem.Size = new System.Drawing.Size(241, 22);
			this.dStrengthToolStripMenuItem.Text = "1D Real Source";
			this.dStrengthToolStripMenuItem.ToolTipText = "Data from real source to complex frequency FFT. File format: double (real), doubl" +
    "e (imaginary); at frequency index 0, 1, 2, ...";
			this.dStrengthToolStripMenuItem.Click += new System.EventHandler(this.d1RealSourceToolStripMenuItem_Click);
			// 
			// dRealSourceInFrequencyToolStripMenuItem
			// 
			this.dRealSourceInFrequencyToolStripMenuItem.Name = "dRealSourceInFrequencyToolStripMenuItem";
			this.dRealSourceInFrequencyToolStripMenuItem.Size = new System.Drawing.Size(241, 22);
			this.dRealSourceInFrequencyToolStripMenuItem.Text = "1D Real Source in frequency";
			this.dRealSourceInFrequencyToolStripMenuItem.Click += new System.EventHandler(this.dRealSourceInFrequencyToolStripMenuItem_Click);
			// 
			// dStrengthToolStripMenuItem1
			// 
			this.dStrengthToolStripMenuItem1.Name = "dStrengthToolStripMenuItem1";
			this.dStrengthToolStripMenuItem1.Size = new System.Drawing.Size(241, 22);
			this.dStrengthToolStripMenuItem1.Text = "1D Strength";
			this.dStrengthToolStripMenuItem1.ToolTipText = "Data from real source to complex frequency FFT. File format: double (real), doubl" +
    "e (imaginary); at frequency index 0, 1, 2, ...";
			this.dStrengthToolStripMenuItem1.Click += new System.EventHandler(this.dStrengthToolStripMenuItem1_Click);
			// 
			// dStrengthNormalizedToolStripMenuItem
			// 
			this.dStrengthNormalizedToolStripMenuItem.Name = "dStrengthNormalizedToolStripMenuItem";
			this.dStrengthNormalizedToolStripMenuItem.Size = new System.Drawing.Size(241, 22);
			this.dStrengthNormalizedToolStripMenuItem.Text = "1D Strength Normalized";
			this.dStrengthNormalizedToolStripMenuItem.ToolTipText = "File format: double (real), double (imaginary); at frequency index 0, 1, 2, ...";
			this.dStrengthNormalizedToolStripMenuItem.Click += new System.EventHandler(this.dStrengthNormalizedToolStripMenuItem_Click);
			// 
			// dStrengthNormalizedByDimToolStripMenuItem
			// 
			this.dStrengthNormalizedByDimToolStripMenuItem.Name = "dStrengthNormalizedByDimToolStripMenuItem";
			this.dStrengthNormalizedByDimToolStripMenuItem.Size = new System.Drawing.Size(241, 22);
			this.dStrengthNormalizedByDimToolStripMenuItem.Text = "1D Strength Normalized by Dim";
			this.dStrengthNormalizedByDimToolStripMenuItem.ToolTipText = "File format: double (real), double (imaginary); at frequency index 0, 1, 2, ...";
			this.dStrengthNormalizedByDimToolStripMenuItem.Click += new System.EventHandler(this.dStrengthNormalizedByDimToolStripMenuItem_Click);
			// 
			// dRelativeStrengthDecreaseToolStripMenuItem
			// 
			this.dRelativeStrengthDecreaseToolStripMenuItem.Name = "dRelativeStrengthDecreaseToolStripMenuItem";
			this.dRelativeStrengthDecreaseToolStripMenuItem.Size = new System.Drawing.Size(241, 22);
			this.dRelativeStrengthDecreaseToolStripMenuItem.Text = "1D Relative Strength Decrease";
			this.dRelativeStrengthDecreaseToolStripMenuItem.ToolTipText = "File format: double (real), double (imaginary); at frequency index 0, 1, 2, ...";
			this.dRelativeStrengthDecreaseToolStripMenuItem.Click += new System.EventHandler(this.dRelativeStrengthDecreaseToolStripMenuItem_Click);
			// 
			// showChartFFTAnalysisToolStripMenuItem
			// 
			this.showChartFFTAnalysisToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.dSpaceFrequencyIndexToolStripMenuItem,
            this.dMaxcenterFrequenciesToolStripMenuItem});
			this.showChartFFTAnalysisToolStripMenuItem.Name = "showChartFFTAnalysisToolStripMenuItem";
			this.showChartFFTAnalysisToolStripMenuItem.Size = new System.Drawing.Size(215, 22);
			this.showChartFFTAnalysisToolStripMenuItem.Text = "FFT Analysis (Chart)";
			// 
			// dSpaceFrequencyIndexToolStripMenuItem
			// 
			this.dSpaceFrequencyIndexToolStripMenuItem.Name = "dSpaceFrequencyIndexToolStripMenuItem";
			this.dSpaceFrequencyIndexToolStripMenuItem.Size = new System.Drawing.Size(215, 22);
			this.dSpaceFrequencyIndexToolStripMenuItem.Text = "1D Space-Frequency Index";
			this.dSpaceFrequencyIndexToolStripMenuItem.ToolTipText = "File format: UInt32 (space index), UInt32 (Frequency index)";
			this.dSpaceFrequencyIndexToolStripMenuItem.Click += new System.EventHandler(this.dSpaceFrequencyIndexToolStripMenuItem_Click);
			// 
			// dMaxcenterFrequenciesToolStripMenuItem
			// 
			this.dMaxcenterFrequenciesToolStripMenuItem.Name = "dMaxcenterFrequenciesToolStripMenuItem";
			this.dMaxcenterFrequenciesToolStripMenuItem.Size = new System.Drawing.Size(215, 22);
			this.dMaxcenterFrequenciesToolStripMenuItem.Text = "1D max-center frequencies";
			this.dMaxcenterFrequenciesToolStripMenuItem.ToolTipText = "File format: double (space location), double (max strength frequency), double (ce" +
    "nter frequency)";
			this.dMaxcenterFrequenciesToolStripMenuItem.Click += new System.EventHandler(this.dMaxcenterFrequenciesToolStripMenuItem_Click);
			// 
			// showCoseToolStripMenuItem
			// 
			this.showCoseToolStripMenuItem.Name = "showCoseToolStripMenuItem";
			this.showCoseToolStripMenuItem.Size = new System.Drawing.Size(215, 22);
			this.showCoseToolStripMenuItem.Text = "Cosine graph";
			this.showCoseToolStripMenuItem.Click += new System.EventHandler(this.showCoseToolStripMenuItem_Click);
			// 
			// toolStripMenuItem1
			// 
			this.toolStripMenuItem1.Name = "toolStripMenuItem1";
			this.toolStripMenuItem1.Size = new System.Drawing.Size(212, 6);
			// 
			// doublesToolStripMenuItem
			// 
			this.doublesToolStripMenuItem.Name = "doublesToolStripMenuItem";
			this.doublesToolStripMenuItem.Size = new System.Drawing.Size(215, 22);
			this.doublesToolStripMenuItem.Text = "Phase Data";
			this.doublesToolStripMenuItem.Click += new System.EventHandler(this.doublesToolStripMenuItem_Click);
			// 
			// highLowRowsToolStripMenuItem
			// 
			this.highLowRowsToolStripMenuItem.Name = "highLowRowsToolStripMenuItem";
			this.highLowRowsToolStripMenuItem.Size = new System.Drawing.Size(215, 22);
			this.highLowRowsToolStripMenuItem.Text = "High Low Rows";
			this.highLowRowsToolStripMenuItem.Click += new System.EventHandler(this.highLowRowsToolStripMenuItem_Click);
			// 
			// toolStripMenuItem2
			// 
			this.toolStripMenuItem2.Name = "toolStripMenuItem2";
			this.toolStripMenuItem2.Size = new System.Drawing.Size(212, 6);
			// 
			// showDataControlToolStripMenuItem
			// 
			this.showDataControlToolStripMenuItem.Name = "showDataControlToolStripMenuItem";
			this.showDataControlToolStripMenuItem.Size = new System.Drawing.Size(215, 22);
			this.showDataControlToolStripMenuItem.Text = "Show Data Control";
			this.showDataControlToolStripMenuItem.Click += new System.EventHandler(this.showDataControlToolStripMenuItem_Click);
			// 
			// eMPerformancesToolStripMenuItem
			// 
			this.eMPerformancesToolStripMenuItem.Name = "eMPerformancesToolStripMenuItem";
			this.eMPerformancesToolStripMenuItem.Size = new System.Drawing.Size(215, 22);
			this.eMPerformancesToolStripMenuItem.Text = "EM Performances";
			this.eMPerformancesToolStripMenuItem.Click += new System.EventHandler(this.eMPerformancesToolStripMenuItem_Click);
			// 
			// FormDraw2D
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(915, 449);
			this.ContextMenuStrip = this.contextMenuStrip1;
			this.Name = "FormDraw2D";
			this.Text = "2D Line Chart";
			this.contextMenuStrip1.ResumeLayout(false);
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.ContextMenuStrip contextMenuStrip1;
		private System.Windows.Forms.ToolStripMenuItem fFTDataFilesToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem dComplexToComplexToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem dRealToComplexToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem dComplexToComplexToolStripMenuItem1;
		private System.Windows.Forms.ToolStripMenuItem dRealToComplexToolStripMenuItem1;
		private System.Windows.Forms.ToolStripMenuItem showChartFFTDataFilesToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem dComplexFileToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem dStrengthToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem dStrengthNormalizedToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem dStrengthNormalizedByDimToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem dRelativeStrengthDecreaseToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem showChartFFTAnalysisToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem dSpaceFrequencyIndexToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem showDataFileInChartToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem showCoseToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem dMaxcenterFrequenciesToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem dStrengthToolStripMenuItem1;
		private System.Windows.Forms.ToolStripMenuItem dRealSourceInFrequencyToolStripMenuItem;
		private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
		private System.Windows.Forms.ToolStripMenuItem doublesToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem highLowRowsToolStripMenuItem;
		private System.Windows.Forms.ToolStripSeparator toolStripMenuItem2;
		private System.Windows.Forms.ToolStripMenuItem showDataControlToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem eMErrorDifferencesToolStripMenuItem;
		private System.Windows.Forms.ToolStripSeparator toolStripMenuItem3;
		private System.Windows.Forms.ToolStripMenuItem addATimefileToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem eMPerformancesToolStripMenuItem;
	}
}

