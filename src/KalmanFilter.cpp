#include "KalmanFilter.h"

KalmanFilter::KalmanFilter()
{
}
void KalmanFilter::begin()
{
    debugln("KalmanFilter begin");
    debug("x_hat: ");
    debug(x_hat(0));
    debug(" ");
    debugln(x_hat(1));
    debug("P: ");
    debug(P(0, 0));
    debug(" ");
    debug(P(0, 1));
    debug(" ");
    debug(P(1, 0));
    debug(" ");
    debug(P(1, 1));
    debugln();

    // initial prediction
    x_hat = (F * x_hat) + (G * u);
    // initial state estimate
    P = (F * P * (~F)) + Q;

    debug("x_hat_1: ");
    debug(x_hat(0));
    debug(" ");
    debugln(x_hat(1));
    debug("P_1: ");
    debug(P(0, 0));
    debug(" ");
    debug(P(0, 1));
    debug(" ");
    debug(P(1, 0));
    debug(" ");
    debug(P(1, 1));
    debugln();
}
struct FilteredValues KalmanFilter::update(SendValues sv)
{
    struct FilteredValues return_val;
    debug("x_hat_loop: ");
    debug(x_hat(0));
    debug(" ");
    debugln(x_hat(1));
    debug("P_loop: ");
    debug(P(0, 0));
    debug(" ");
    debug(P(0, 1));
    debug(" ");
    debug(P(1, 0));
    debug(" ");
    debug(P(1, 1));
    debugln();
    debug("Q: ");
    debug(Q(0, 0));
    debug(" ");
    debug(Q(0, 1));
    debug(" ");
    debug(Q(1, 0));
    debug(" ");
    debug(Q(1, 1));
    debugln();

    /*................measurements ...............*/
    Matrix<1> z = {sv.altitude};
    debugln("z: ");
    debug(z(0));
    debugln();

    if (abs(sv.ax) > 8 || abs(sv.az) > 8)
    {
        u = abs(sv.ay);
    }
    else
    {
        u = abs(sv.ay) - gravity;
    }
    debug("u: ");
    debug(u);
    debugln();
    /*................... update ...................*/
    // Kalman gain
    Matrix<1> con = ((H * P * (~H)) + R);
    debug("con: ");
    debug(con(0));
    debugln();
    debug("inv: ");
    debug(1 / con(0));
    debugln();
    Matrix<2> K = (P * (~H)) * (1 / con(0));
    debug("K: ");
    debug(K(0));
    debug(" ");
    debug(K(1));
    debugln();
    debug("H_rows: ");
    debug(H.Rows);
    debugln();
    debug(" H_cols: ");
    debug(H.Cols);
    debugln();
    // estimate current state
    x_hat = x_hat + K * (z - (H * x_hat));
    debug("x_hat_current_1: ");
    debug(x_hat(0));
    debug(" ");
    debugln(x_hat(1));

    // estimate current uncertainty
    P = (I - K * H) * P * (~(I - K * H)) + K * R * (~K);
    debug("P_current_1: ");
    debug(P(0, 0));
    debug(" ");
    debug(P(0, 1));
    debug(" ");
    debug(P(1, 0));
    debug(" ");
    debug(P(1, 1));
    debugln();

    /* .............predict................*/

    x_hat = (F * x_hat) + (G * u);
    P = (F * P * (~F)) + Q;

    return_val.displacement = x_hat(0);
    return_val.velocity = x_hat(1);
    return_val.acceleration = u;

    return return_val;
}