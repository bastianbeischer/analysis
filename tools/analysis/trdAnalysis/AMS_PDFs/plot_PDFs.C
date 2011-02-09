#include "pfun.C"

void plot_PDFs(){

  TF1 *f_epar=new TF1("f_epar","(landau(0)+landau(3))*expo(6)");
  TF1 *f_ppar=new TF1("fit_p",pfun,0.,100,7);

  f_epar->SetParameters(0.533941,2.04519,0.90618,0.177358,13.3248,4.80789,0.312408,-0.0432751);
  f_ppar->SetParameters(1.32391,2.23676,1.02281,0.788797,-0.0594475,0.00140946,-2.07114e-05);

  f_epar->SetLineColor(2);
  f_ppar->SetLineColor(4);
  f_ppar->SetRange(0,80);
  f_epar->SetRange(0,80);

	f_ppar->SetNpx(1000);
	f_epar->SetNpx(1000);
  f_ppar->Draw();
  f_epar->Draw("same");
  f_ppar->GetXaxis()->SetTitle("amplitude [keV]");
  f_ppar->GetYaxis()->SetTitle("Probability");
  TLegend *leg=new TLegend(0.6,0.6,0.85,0.85);
  leg->AddEntry(f_ppar,"proton PDF","l");
  leg->AddEntry(f_epar,"electron PDF","l");
  leg->Draw("same");
}
