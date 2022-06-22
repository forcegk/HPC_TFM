add_cus_dep('glo', 'gls', 0, 'run_makeglossaries');
add_cus_dep('acn', 'acr', 0, 'run_makeglossaries');

sub run_makeglossaries {
  if ( $silent ) {
    system "makeglossaries -q '$_[0]'";
  }
  else {
    system "makeglossaries '$_[0]'";
  };
}

$pdflatex = 'pdflatex -interaction=nonstopmode';

$pdf_previewer = '';
$clean_ext .= 'acn acr alg aux bbl fdb_latexmk fls glg* glo* gls* idx ilg ' .
              'ind ist nav nlo nls nlg loc lof lot log out pyg pytxcode run.xml slo ' .
              'sls slg snm soc synctex.gz tdo thm toc upa vrb xdy _minted-%R/* _minted-%R ' .
              'pythontex-files-%R *-eps-converted-to.pdf *.gnuplot';
