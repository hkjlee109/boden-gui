addToLibrary({

    lib_begin_draw: function() {
        console.log("# begin_draw");     
    },

    lib_end_draw: function(
        commands_addr, commands_count,
        indices_addr, indices_count,
        vertices_addr, vertices_count
    ) {
        console.log("# end_draw");

        const canvas = document.querySelector("#gl-canvas");
        const gl = canvas.getContext("webgl");
    
        if(gl === null) {
            alert("Unable to initialize WebGL. Your browser or machine may not support it.");
            return;
        }
    
        gl.clearColor(0.0, 0.0, 1.0, 1.0);
        gl.clear(gl.COLOR_BUFFER_BIT);
    }
    
});
    