# JT_UVTransform for Nuke

This plugin adds a node that allows you to apply simple transforms to the UV coordinates of the input meshes

## Building the plugins

Go to the src directory, and there:

	make -f Makefile.mac NDKDIR=/Applications/Nuke6.3v1/Nuke6.3v1.app/Contents/MacOS # for MacOS
	make -f Makefile.lin NDKDIR=/mnt/server/thefoundry/Nuke6.3v1/Nuke6.3v1 # for Linux
 
## Testing the plugin

See the sample script.

## Credits

The plugin has been made by Julik Tarkhanov. The pharmacy panorama has been generously provided by
[JP Leclercq](http://jplsfx.wix.com/jeanphilippeleclercq#!pano_maroc/c14ba)

## Questions and remarks

For questions and comments shoot a mail to me \_at\_ julik.nl

## License

    Copyright (c) 2012 Julik Tarkhanov
    
    	Permission is hereby granted, free of charge, to any person obtaining
    	a copy of this software and associated documentation files (the
    	"Software"), to deal in the Software without restriction, including
    	without limitation the rights to use, copy, modify, merge, publish,
    	distribute, sublicense, and/or sell copies of the Software, and to
    	permit persons to whom the Software is furnished to do so, subject to
    	the following conditions:
    
    	The above copyright notice and this permission notice shall be
    	included in all copies or substantial portions of the Software.
    
    	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
    	LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
    	OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
    	WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 [1]: http://www.ssontech.com/content/lensalg.htm
 [2]: https://github.com/julik/sylens/raw/master/images/kappa_value.png
 [3]: https://github.com/julik/sylens/raw/master/images/auto_disto.png
 [4]: https://github.com/julik/sylens/raw/master/images/just_k.png
 [5]: https://github.com/julik/sylens/raw/master/images/SyLens_Figures_03.png
 [6]: https://github.com/julik/sylens/raw/master/images/projection_dag.png
 [7]: #sycamera
 [8]: https://github.com/julik/sylens/raw/master/images/syuv_dag.png
 [9]: #iop
 [10]: https://github.com/julik/sylens/raw/master/images/syuv_undisto.png
 [11]: https://github.com/julik/sylens/raw/master/images/syuv_controls.png
 [12]: https://github.com/julik/sylens/raw/master/images/cam_std.png
 [13]: https://github.com/julik/sylens/raw/master/images/cam_sy.png
 [14]: https://github.com/julik/sylens/raw/master/images/cam_controls.png
 [15]: https://github.com/julik/sylens/raw/master/images/cam_few_subdivs.png
 [16]: https://github.com/julik/sylens/raw/master/images/cam_many_subdivs.png
 [17]: https://github.com/julik/sylens/raw/master/images/sylens_controls.png
 [18]: https://github.com/julik/sylens/raw/master/images/sygeo.png
 [19]: https://github.com/julik/sylens/raw/master/images/local_space.png
 [20]: https://github.com/julik/sylens/raw/master/images/global_space.png