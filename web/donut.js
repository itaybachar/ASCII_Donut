//Canvas Variables
let canvas = document.getElementById('canvas');
let frameRefresh = undefined;
let ctx = undefined;
let WIDTH = 0, HEIGHT = 0;

//Donut Variables
let R1 = 1, R2 = 2, K1 = 150, K2 = 9;
//Rotation Variables
let A = Math.PI / 2, B = Math.PI / 2;

const clearFrame = function ()
{
    //set background
    ctx.fillStyle = '#000';
    ctx.fillRect(0, 0, ctx.canvas.width, ctx.canvas.height);
}
const drawFrame = function ()
{
    clearFrame();

    //Precompute some values
    let c_A = Math.cos(A), c_B = Math.cos(B);
    let s_A = Math.sin(A), s_B = Math.sin(B);

    for (let theta = 0; theta < 3.14 * 2; theta += 0.1)
    {
        let s_theta = Math.sin(theta), c_theta = Math.cos(theta);
        for (let phi = 0; phi < 2 * 3.14; phi += 0.1)
        {
            let s_phi = Math.sin(phi), c_phi = Math.cos(phi);

            //Calculate shape x,y
            let shape_x = R2 + R1 * c_theta;
            let shape_y = R1 * s_theta;

            //Calculate 3d
            let x = s_B * (shape_y * c_A + s_A * s_phi * shape_x) + c_B * c_phi * shape_x;
            let y = c_B * (shape_y * c_A + s_A * s_phi * shape_x) - s_B * c_phi * shape_x;
            let z = K2 + c_A * s_phi * shape_x - shape_y * s_A;
            let ooz = 1 / z; //one over z

            //Screen coordinates finally
            let xp = WIDTH / 2 + K1 * x * ooz;
            let yp = HEIGHT / 2 - K1 * y * ooz;

            //Find Lum
            let lum = -c_A * c_theta * s_phi + s_A * s_theta - c_B * c_A * s_theta - c_B * s_A * c_theta * s_phi + s_B * c_theta * c_phi;
            lum *= 0.7; //we multiply by 0.7 to normalize to 0-1. The result of Lum is 0->sqrt2
            if (lum > 0) // if object is facing us.
            {
                //Draw
                ctx.fillStyle = 'rgba(255,255,255,' + lum + ')'
            } else
            {
                ctx.fillStyle = 'rgba(255,0,0,' + -lum / 2 + ')'

            }
            ctx.fillRect(xp, yp, 1.5, 1.5);
        }
    }
    //Update A,B
    A += 0.1;
    B += 0.05;
}

const startDraw = function ()
{
    canvas = document.getElementById('canvas');
    if (canvas.getContext)
    {
        ctx = canvas.getContext("2d");

        WIDTH = ctx.canvas.width;
        HEIGHT = ctx.canvas.height;
        //setup timer for frame rate
        if (frameRefresh == undefined)
        {
            frameRefresh = setInterval(drawFrame, 50);
        }
    }
};