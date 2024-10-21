'''
Originally a Jupyter notebook, this file has been converted to a Python script
for the web formatting
'''

# Setup the Jupyter version of Dash
from jupyter_dash import JupyterDash

# Configure the necessary Python module imports for dashboard components
import dash_leaflet as dl
from dash import dcc
from dash import html
from dash import no_update  # , ctx
import plotly.express as px
from dash import dash_table
from dash.dependencies import Input, Output
import base64

# Configure OS routines
import os

# Configure the plotting routines
import pandas as pd

# Utility
from dataclasses import dataclass, field
from typing import Union

# CRUD module
from animal_shelter import AnimalShelter

###########################
# Data Manipulation / Model
###########################

username = "aacuser"
password = "super_secret_password_123"

# Connect to database via CRUD Module
db = AnimalShelter(USER=username, PASS=password)

# class read method must support return of list object and accept projection json input
# sending the read method an empty document requests all documents be returned
df = pd.DataFrame.from_records(db.read({}))

# MongoDB v5+ is going to return the '_id' column and that is going to have an
# invlaid object type of 'ObjectID' - which will cause the data_table to crash - so we remove
# it in the dataframe here. The df.drop command allows us to drop the column. If we do not set
# inplace=True - it will reeturn a new dataframe that does not contain the dropped column(s)
df.drop(columns=["_id"], inplace=True)

## Filter data

Numeric = Union[int, float]


@dataclass(frozen=True)
class FilterConfig:
    """
    Configuration for each filter

    Attributes:
        rescueType (str): The label used
        preferredBreeds (set[str]): The `set` of breeds to include in the filter
        preferredSex (str): The sex to include in the filter
        trainingAge (tuple[Numeric, Numeric]): The age range to include in the filter. `Numeric` is an `int` or `float`. `(lower bound, upper bound)`
    """

    rescueType: str
    preferredBreeds: set[str]
    preferredSex: str
    trainingAge: tuple[Numeric, Numeric]


filter_configs: tuple[FilterConfig, ...] = (
    FilterConfig(
        rescueType="Water Rescue",
        preferredBreeds=set(
            [
                "Labrador Retriever Mix",
                "Chesapeake Bay Retriever",
                "Newfoundland",
            ]
        ),
        preferredSex="Intact Female",
        trainingAge=(26, 156),
    ),
    FilterConfig(
        rescueType="Mountain or Wilderness Rescue",
        preferredBreeds=set(
            [
                "German Shepherd",
                "Alaskan Malamute",
                "Old English Sheepdog",
                "Siberian Husky",
                "Rottweiler",
            ]
        ),
        preferredSex="Intact Male",
        trainingAge=(26, 156),
    ),
    FilterConfig(
        rescueType="Disaster or Individual Tracking",
        preferredBreeds=set(
            [
                "Doberman Pinscher",
                "German Shepherd",
                "Golden Retriever",
                "Bloodhound",
                "Rottweiler",
            ]
        ),
        preferredSex="Intact Male",
        trainingAge=(20, 300),
    ),
)

#########################
# Dashboard Layout / View
#########################
app = JupyterDash(__name__)

logo_filename = "Grazioso Salvare Logo.png"
encoded_image = (
    base64.b64encode(open(logo_filename, "rb").read())
    if os.path.exists(logo_filename)
    else bytes("")
)


@dataclass
class CheckboxConfig:
    label: str
    componentId: str = field(init=False)

    def __post_init__(self):
        self.componentId = f"filter-{self.label.lower()}"


app.layout = html.Div(
    [
        html.Center(html.B(html.H1("CS-340 Dashboard"))),
        html.Hr(),
        html.Div(
            children=[
                html.Label("Filter by:"),
                dcc.Checklist(
                    [filter.rescueType for filter in filter_configs],
                    id="filter-list",
                    inline=True,
                ),
                html.Button("Reset", id="reset-filters", n_clicks=0),
            ]
        ),
        html.Hr(),
        dash_table.DataTable(
            id="datatable-id",
            columns=[
                {"name": i, "id": i, "deletable": False, "selectable": True}
                for i in df.columns
            ],
            data=df.to_dict("records"),
            row_selectable="single",
            page_size=10,
            style_table={"overflow-x": "auto"},
        ),
        html.Br(),
        html.Hr(),
        # This sets up the dashboard so that your chart and your geolocation chart are side-by-side
        html.Div(
            className="row",
            style={"display": "flex"},
            children=[
                html.Div(
                    id="graph-id",
                    className="col s12 m6",
                ),
                html.Div(
                    id="map-id",
                    className="col s12 m6",
                ),
            ],
        ),
        # Render logo image and identifier on the same line
        html.Div(
            style={
                "display": "flex",
                "justify-content": "space-around",
                "align-items": "center",
            },
            children=[
                html.A(
                    children=[
                        html.Img(
                            src="data:image/png;base64,{}".format(
                                encoded_image.decode()
                            ),
                            alt="Grazioso Salvare Logo",
                        )
                    ],
                    href="https://www.snhu.edu/",
                ),
                html.H1(
                    children=[
                        "Dennis James Stelmach",
                        html.Br(),
                        "Project Two",
                        html.Br(),
                        "2024-02-25",
                    ],
                    style={"text-align": "center"},
                ),
            ],
        ),
    ]
)

#############################################
# Interaction Between Components / Controller
#############################################


@app.callback(
    Output("filter-list", "value"),
    [Input("reset-filters", "n_clicks")],
)
def reset_checklist(n_clicks):
    """Unset all filters, resetting the dashboard."""

    if n_clicks > 0:
        return []
    return no_update


@app.callback(
    Output("datatable-id", "data"),
    [Input("filter-list", "value")],
)
def update_dashboard(filters):
    """Update the DataTable based on the filter type on filter change."""

    # Get filter configs that are selected
    filters_to_use = filter(lambda f: f.rescueType in filters, filter_configs)

    # Build filter blocks for selected configs
    filter_blocks = [
        {
            "breed": {"$in": list(filter_config.preferredBreeds)},
            "sex_upon_outcome": filter_config.preferredSex,
            "age_upon_outcome_in_weeks": {
                "$gte": filter_config.trainingAge[0],
                "$lte": filter_config.trainingAge[1],
            },
        }
        for filter_config in filters_to_use
    ]

    df = pd.DataFrame.from_records(
        # Find with our filters joined by `or`, or grab everything if no filters
        db.read({"$or": filter_blocks} if len(filter_blocks) > 0 else {})
    )
    # Drop the _id column as the DataTable doesn't serialize it.
    if "_id" in df.columns:
        df.drop(columns=["_id"], inplace=True)

    data = df.to_dict("records")

    return data


@app.callback(
    Output("graph-id", "children"), [Input("datatable-id", "derived_virtual_data")]
)
def update_graphs(viewData):
    """Display the breeds of animal based on quantity represented in the data table."""

    dff = pd.DataFrame.from_dict(viewData)
    if dff.size == 0:
        return no_update

    pie_chart = px.pie(dff, names="breed", title="Preferred Animals")

    return [dcc.Graph(figure=pie_chart)]


@app.callback(
    Output("datatable-id", "style_data_conditional"),
    [Input("datatable-id", "selected_columns")],
)
def update_styles(selected_columns):
    """Highlight a cell on the data table when the user selects it"""

    return [
        {"if": {"column_id": i}, "background_color": "#D2F3FF"}
        for i in selected_columns or []
    ]


@app.callback(
    Output("map-id", "children"),
    [
        Input("datatable-id", "derived_virtual_data"),
        Input("datatable-id", "derived_virtual_selected_rows"),
    ],
)
def update_map(viewData, index):
    """
    This callback will update the geo-location chart for the selected data entry
    derived_virtual_data will be the set of data available from the datatable in the form of
    a dictionary.
    derived_virtual_selected_rows will be the selected row(s) in the table in the form of
    a list. For this application, we are only permitting single row selection so there is only
    one value in the list.
    The iloc method allows for a row, column notation to pull data from the datatable
    """

    def new_base_map():
        return dl.Map(
            style={"width": "1000px", "height": "500px"},
            center=[30.75, -97.48],
            zoom=10,
        )

    if viewData is None:
        return [new_base_map()]

    dff = pd.DataFrame.from_dict(viewData)
    if dff.size == 0:
        return [new_base_map()]

    # Because we only allow single row selection, the list can be converted to a row index here
    if index is None or len(index) == 0:
        row = 0
    else:
        row = index[0]

    # Austin TX is at [30.75,-97.48]
    return [
        dl.Map(
            style={"width": "1000px", "height": "500px"},
            center=[30.75, -97.48],
            zoom=10,
            children=[
                dl.TileLayer(id="base-layer-id"),
                # Marker with tool tip and popup
                # Column 13 and 14 define the grid-coordinates for the map
                # Column 4 defines the breed for the animal
                # Column 9 defines the name of the animal
                dl.Marker(
                    position=[dff.iloc[row, 13], dff.iloc[row, 14]],
                    children=[
                        dl.Tooltip(dff.iloc[row, 4]),
                        dl.Popup([html.H1("Animal Name"), html.P(dff.iloc[row, 9])]),
                    ],
                ),
            ],
        )
    ]


app.run_server(debug=True)